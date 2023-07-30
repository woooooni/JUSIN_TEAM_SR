#include "Particle_MovingLeaf.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"


CParticle_MovingLeaf::CParticle_MovingLeaf(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_EFFECT, OBJ_ID::EFFECT)
{
}

CParticle_MovingLeaf::CParticle_MovingLeaf(const CParticle_MovingLeaf& rhs)
	: CGameObject(rhs)
{
}

CParticle_MovingLeaf::~CParticle_MovingLeaf()
{
}

HRESULT CParticle_MovingLeaf::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.1f));

	m_pAnimator->Add_Animation(L"MovingLeaf", L"Proto_Texture_Effect_LeafFloating", 0.1f);

	m_pAnimator->Play_Animation(L"MovingLeaf", true);

	Set_Active(false);

	m_fMinHeight = 0.2f;
	m_pRigidBodyCom->SetGravity(false);

	m_fAccTime = 0.0f;
	m_fEndTime = 1.0f;
	m_iAlpha = m_iR = m_iG = m_iB = 255;

	Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Moving_Leaf", this);

	return S_OK;
}

_int CParticle_MovingLeaf::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;



	if (m_fAccTime > m_fEndTime)
	{
		m_iAlpha -= 5;
		if (m_iAlpha == 0)
		{
			CPool<CParticle_MovingLeaf>::Return_Obj(this);
		}
	}
	else
		m_fAccTime += fTimeDelta;


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CParticle_MovingLeaf::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;




	__super::LateUpdate_Object();
}

void CParticle_MovingLeaf::Render_Object(void)
{
	if (!Is_Active())
		return;

	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { m_iR / 255.0f, m_iG / 255.0f, m_iB / 255.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

HRESULT CParticle_MovingLeaf::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CParticle_MovingLeaf::Random_Particle(_vec3& _vPos)
{
	if (_vPos.y < 0.3f)
		_vPos.y = 0.3f;

	m_iAlpha = m_iR = m_iG = m_iB = 255;

	_int iColor = rand() % 2;
	if (iColor == 1)
	{
		m_iR = 14;
		m_iG = 209;
		m_iB = 69;
	}

	m_fAccTime = 0.0f;
	m_pRigidBodyCom->SetGround(false);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	_vPos.z -= 0.1f;
	_float fScale = 0.1f + ((rand() % 10) * 0.02f);
	_float fAngle = _float(rand() % 360);
	_float fForceX = (rand() % 21) * 0.4f;
	_float fForceY = ((rand() % 10) * 0.4f) + 10.0f;
	_uint iIndex = rand() % 8;

	m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);

	_int iX = rand() % 2;
	if (iX == 1)
		fForceX *= -1.0f;

	m_pTransformCom->Set_Scale(_vec3(fScale, fScale, 0.0f));
	m_pTransformCom->RotationAxis(_vec3(0.0f, 0.0f, 1.0f), fAngle);
	m_pTransformCom->Set_Pos(&_vPos);

	m_pRigidBodyCom->AddForce(_vec3(fForceX, fForceY, 0.0f));
}

CParticle_MovingLeaf* CParticle_MovingLeaf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle_MovingLeaf* pInstance = new CParticle_MovingLeaf(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Particle_MovingLeaf_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle_MovingLeaf::Free()
{
	__super::Free();
}
