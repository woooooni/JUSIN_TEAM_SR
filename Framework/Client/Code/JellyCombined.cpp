#include "JellyCombined.h"
#include "Export_Function.h"
#include	"JellyStone.h"
#include	<time.h>
#include	"Pool.h"

CJellyCombined::CJellyCombined(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::JELLY_COMBINED), m_eColor(JELLY_COLLOR_COMBINE::JELLY_COMBINEEND)
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

	CComponent* pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ COMPONENT_TYPE::COM_RIGIDBODY, pComponent });

	Set_MinHeight(0.5f);

	return S_OK;
}

_int CJellyCombined::Update_Object(const _float& fTimeDelta)
{
	if (Is_Active())
	{
		Add_RenderGroup(RENDER_ALPHA, this);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);

	}


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
	ret->m_pAnimator->Add_Animation(L"Red", L"Proto_Tex_Jelly_Red", 1.f);
	ret->m_pAnimator->Add_Animation(L"Green", L"Proto_Tex_Jelly_Green", 1.f);
	ret->m_pAnimator->Add_Animation(L"Blue", L"Proto_Tex_Jelly_Blue", 1.f);


	switch (ret->m_eColor)
	{	
	case JELLY_COLLOR_COMBINE::RED :
		ret->m_pAnimator->Play_Animation(L"Red", false);
		break;
	case JELLY_COLLOR_COMBINE::GREEN:
		ret->m_pAnimator->Play_Animation(L"Green", false);

		break;
	case JELLY_COLLOR_COMBINE::BLUE:
		ret->m_pAnimator->Play_Animation(L"Blue", false);

		break;


	default:
		break;
	}
	ret->Set_SubscribeEvent(p_EventNum);
	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CJellyCombined::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyCombined::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
		return;


	Push_Me(pCollider);
}

void CJellyCombined::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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

CGameObject* CJellyCombined::Get_GrabObj()
{
	CPool<CJellyCombined>::Return_Obj(this);

	_vec3 dst;

	m_pTransformCom->Get_Info(INFO_POS, &dst);

	CJellyStone* iter = dynamic_cast<CJellyStone*>(CPool<CJellyStone>::Get_Obj());
	if (iter == nullptr)
	{
		iter = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::JELLY_NORMALEND);
		NULL_CHECK_RETURN_MSG(iter, nullptr, L"JellyGrabFailed");
		Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly_Normal", iter);
	}

	CJellyStone* tmp = dynamic_cast<CJellyStone*>(CPool<CJellyStone>::Get_Obj());
	if (tmp == nullptr)
		{
			tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::JELLY_NORMALEND);
			NULL_CHECK_RETURN_MSG(tmp, nullptr, L"JellyGrabFailed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly_Normal", tmp);
		}



	(iter)->Get_TransformCom()->Set_Pos(&_vec3(dst.x, 0.5f, dst.z));
	(tmp)->Get_TransformCom()->Set_Pos(&_vec3(dst.x, 0.5f, dst.z));
	(iter)->Set_Active(true);
	(tmp)->Set_Active(true);
	iter->Set_Created_False();
	tmp->Set_Created_False();

	switch (m_eColor)
	{
	case Engine::JELLY_COLLOR_COMBINE::BLUE:
		iter->Set_Color(JELLY_COLLOR_NORMAL::CYAN);
		tmp->Set_Color(JELLY_COLLOR_NORMAL::MAGENTA);
		break;
	case Engine::JELLY_COLLOR_COMBINE::GREEN:
		iter->Set_Color(JELLY_COLLOR_NORMAL::CYAN);
		tmp->Set_Color(JELLY_COLLOR_NORMAL::YELLOW);

		break;
	case Engine::JELLY_COLLOR_COMBINE::RED:
		iter->Set_Color(JELLY_COLLOR_NORMAL::YELLOW);
		tmp->Set_Color(JELLY_COLLOR_NORMAL::MAGENTA);

		break;
	case Engine::JELLY_COLLOR_COMBINE::JELLY_COMBINEEND:
		MSG_BOX("Grab Failed");
		return nullptr;
	default:
		MSG_BOX("Grab Failed");
		return nullptr;
	}

	if (rand() % 2)
		return iter;
	else
		return tmp;
}

void CJellyCombined::Set_SubscribeEvent(_uint pEvent)
{
	Add_Subscribe(pEvent, this);
}

void CJellyCombined::Set_JellyColor(const JELLY_COLLOR_COMBINE& pColor)
{
	m_eColor = pColor;

	switch (m_eColor)
	{
	case JELLY_COLLOR_COMBINE::RED:

		m_pAnimator->Play_Animation(L"Red", false);
		break;
	case JELLY_COLLOR_COMBINE::GREEN:
		m_pAnimator->Play_Animation(L"Green", false);

		break;
	case JELLY_COLLOR_COMBINE::BLUE:
		m_pAnimator->Play_Animation(L"Blue", false);

	case Engine::JELLY_COLLOR_COMBINE::JELLY_COMBINEEND:
		break;
	default:
		break;
	}
}

bool CJellyCombined::Check_Child(const JELLY_COLLOR_NORMAL& pColor)
{
	switch (m_eColor)
	{
	case Engine::JELLY_COLLOR_COMBINE::BLUE:
		if (pColor != JELLY_COLLOR_NORMAL::YELLOW)
			return false;
		else
			return true;
		break;
	case Engine::JELLY_COLLOR_COMBINE::GREEN:
		if (pColor != JELLY_COLLOR_NORMAL::MAGENTA)
			return false;
		else
			return true;
		break;
	case Engine::JELLY_COLLOR_COMBINE::RED:
		if (pColor != JELLY_COLLOR_NORMAL::CYAN)
			return false;
		else
			return true;
		break;

	default:
		return false;
		break;
	}
}
