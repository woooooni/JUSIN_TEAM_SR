#include "FistEffect.h"
#include "Export_Function.h"
#include "Pool.h"

CFistEffect::CFistEffect(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CEffect(pGraphicDev)
{
}
CFistEffect::CFistEffect(const CFistEffect& rhs)
	: CEffect(rhs)
{

}

CFistEffect::~CFistEffect()
{
}

HRESULT CFistEffect::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"FistEffect", L"Proto_Texture_Effect_FistEffect", 0.1f);
	m_pAnimator->Play_Animation(L"FistEffect", true);

	m_pTransformCom->Set_Pos(&_vec3(-5.0f, 1.1f, 0.0f));
	m_pTransformCom->Set_Scale({ 2.f, 2.f, 2.f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 2.f, 2.f, 2.f });
	_vec3 AxisX = { 1,0,0 };
	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
	Set_Active(true);
	return S_OK;
}

_int CFistEffect::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK; 
	int iExit = __super::Update_Object(fTimeDelta);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_EFFECT);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (Is_Active())
			Set_Active(false);
	}

	return iExit;
}

void CFistEffect::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::LateUpdate_Object();


}

void CFistEffect::Render_Object(void)
{
	if (!Is_Active())
		return;

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
	pEffect->SetFloat("g_AlphaRef", 0.f);


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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

void CFistEffect::Return_Pool(void)
{
}

HRESULT CFistEffect::Add_Component(void)
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
	
	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}


CFistEffect* CFistEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFistEffect* pInstance = new CFistEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FistEffect Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CFistEffect::Free()
{
	__super::Free();
}