#include "Effect_Dig.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Dig::CEffect_Dig(LPDIRECT3DDEVICE9 pGraphicDev)
    :CEffect(pGraphicDev)
{

}

CEffect_Dig::CEffect_Dig(const CEffect& rhs)
    : CEffect(rhs)
{
}

CEffect_Dig::~CEffect_Dig()
{
}

HRESULT CEffect_Dig::Ready_Object(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pAnimator->Add_Animation(L"Dig", L"Proto_Texture_Effect_DigEffect", 0.05f);

    m_pAnimator->Play_Animation(L"Dig", false);

    Set_Active(false);

    return S_OK;
}

_int CEffect_Dig::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;


	

	if (m_fAccTime > m_fEffectTime)
	{
		m_iAlpha -= 1;
		if (m_iAlpha < 10)
		{
			m_iAlpha = 0;
			Set_Active(false);
			CPool<CEffect_Dig>::Return_Obj(this);
		}
	}
	else
		m_fAccTime += fTimeDelta;


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Dig::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;


	__super::LateUpdate_Object();
}

void CEffect_Dig::Render_Object(void)
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

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


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

void CEffect_Dig::Return_Pool(void)
{
	Set_Active(false);
	CPool<CEffect_Dig>::Return_Obj(this);
}

CEffect_Dig* CEffect_Dig::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Dig* pInstance = new CEffect_Dig(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Dig Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Dig::Get_Effect(_vec3& _vPos, _vec3& _vScale, _float fTime)
{
	m_iAlpha = 255;
	m_fAccTime = 0.0f;
	m_fEffectTime = fTime;
	_vPos.y = 0.06f;

	_float m_fAngle;

	m_fAngle = D3DXToRadian(90.0f);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fAngle);

	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"Dig", this);
}

HRESULT CEffect_Dig::Add_Component(void)
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CEffect_Dig::Free()
{
	__super::Free();
}
