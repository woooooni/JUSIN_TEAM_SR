#include "JellyCombined.h"
#include "Export_Function.h"

CJellyCombined::CJellyCombined(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev), m_eColor(JELLY_COLLOR_COMBINE::JELLY_COMBINEEND)
{
	m_tInfo.m_bIsPushable = true;
	m_tInfo.m_bIsGrabbable = true;

}

CJellyCombined::CJellyCombined(const CJellyCombined& rhs) : CFieldObject(rhs), m_eColor(rhs.m_eColor)
{
}

CJellyCombined::~CJellyCombined()
{
}

HRESULT CJellyCombined::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	return S_OK;
}

_int CJellyCombined::Update_Object(const _float& fTimeDelta)
{

	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);


	return __super::Update_Object(fTimeDelta);
}

void CJellyCombined::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CJellyCombined::Render_Object(void)
{
	__super::Render_Object();
}

void CJellyCombined::Free()
{
	__super::Free();
}

CJellyCombined* CJellyCombined::Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLLOR_COMBINE p_Color, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CJellyCombined* ret = new CJellyCombined(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("JellyCombineCreateFailed");
		return nullptr;
	}

	ret->m_eColor = p_Color;
	switch (ret->m_eColor)
	{	
	case JELLY_COLLOR_COMBINE::RED :
		ret->m_pAnimator->Add_Animation(L"Red", L"Proto_Tex_Jelly_Red", 1.f);
		ret->m_pAnimator->Play_Animation(L"Red", false);
		break;
	case JELLY_COLLOR_COMBINE::GREEN:
		ret->m_pAnimator->Add_Animation(L"Green", L"Proto_Tex_Jelly_Green", 1.f);
		ret->m_pAnimator->Play_Animation(L"Green", false);

		break;
	case JELLY_COLLOR_COMBINE::BLUE:
		ret->m_pAnimator->Add_Animation(L"Blue", L"Proto_Tex_Jelly_Blue", 1.f);
		ret->m_pAnimator->Play_Animation(L"Blue", false);

		break;


	default:
		break;
	}
	ret->Set_SubscribeEvent(p_EventNum);
	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CJellyCombined::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{

}

void CJellyCombined::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CJellyCombined::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

HRESULT CJellyCombined::Ready_Component()
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);




	return S_OK;

}

void CJellyCombined::Event_Start(_uint iEventNum)
{
}

void CJellyCombined::Event_End(_uint iEventNum)
{
}

void CJellyCombined::Set_SubscribeEvent(_uint pEvent)
{
	Add_Subscribe(pEvent, this);
}
