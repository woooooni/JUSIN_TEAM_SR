#include "FistEffect.h"
#include "Export_Function.h"

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
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

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