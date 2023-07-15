#include "Particle_FixedLeaf.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"


CParticle_FixedLeaf::CParticle_FixedLeaf(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_EFFECT, OBJ_ID::EFFECT)
{
}

CParticle_FixedLeaf::CParticle_FixedLeaf(const CParticle_FixedLeaf& rhs)
	: CGameObject(rhs)
{
}

CParticle_FixedLeaf::~CParticle_FixedLeaf()
{
}

HRESULT CParticle_FixedLeaf::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.1f));

	m_pAnimator->Add_Animation(L"FixedLeaf", L"Proto_Texture_Effect_FixedLeaf", 0.1f);

	m_pAnimator->Play_Animation(L"FixedLeaf", false);

	Set_Active(false);

	m_fMinHeight = 0.2f;
	m_pRigidBodyCom->SetGravity(-2.5f);

	m_fAccTime = 0.0f;
	m_fEndTime = 1.0f;
	m_iAlpha = m_iR = m_iG = m_iB = 255;

	// Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Fixed_Leaf", this);

	return S_OK;
}

_int CParticle_FixedLeaf::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	

	if (m_fAccTime > m_fEndTime)
	{
		m_iAlpha -= 5;
		if (m_iAlpha == 0)
		{
			CPool<CParticle_FixedLeaf>::Return_Obj(this);
		}
	}
	else
		m_fAccTime += fTimeDelta;


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CParticle_FixedLeaf::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;




	__super::LateUpdate_Object();
}

void CParticle_FixedLeaf::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_iR, m_iG, m_iB));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CParticle_FixedLeaf::Ready_Component(void)
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

	return S_OK;
}

void CParticle_FixedLeaf::Random_Particle(_vec3& _vPos)
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
	_float fScale = 0.15f + ((rand() % 10) * 0.02f);
	_float fAngle = _float(rand() % 360);
	_float fForceX = (rand() % 21) * 0.5f;
	_float fForceY = ((rand() % 10) * 1.2f) + 20.0f;

	_int iX = rand() % 2;
	if (iX == 1)
		fForceX *= -1.0f;

	m_pTransformCom->Set_Scale(_vec3(fScale, fScale, 0.0f));
	m_pTransformCom->RotationAxis(_vec3(0.0f, 0.0f, 1.0f), fAngle);
	m_pTransformCom->Set_Pos(&_vPos);
	
	m_pRigidBodyCom->AddForce(_vec3(fForceX, fForceY, 0.0f));

	
}

CParticle_FixedLeaf* CParticle_FixedLeaf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle_FixedLeaf* pInstance = new CParticle_FixedLeaf(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Particle_FixedLeaf_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle_FixedLeaf::Free()
{
	__super::Free();
}
