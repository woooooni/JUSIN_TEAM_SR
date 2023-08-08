#include "TriggerObj.h"
#include	"Export_Function.h"
#include "UIMgr.h"

CTriggerObj::CTriggerObj(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_INTERACTION, OBJ_ID::NEAR_REACT)
, m_bIsOnce(false)
,m_pTarget(nullptr)
, m_pLateEvent (nullptr)
, m_bHasDialogue(false)
, m_bIsUpdateDialogue(false)
{
	for (_uint i = 0; i < (_uint)COLLIDE_EVENT_TYPE::TYPE_END; i++)
	{
		m_pCollideEvent[i] = nullptr;
	}
}

CTriggerObj::CTriggerObj(const CTriggerObj& rhs) : CGameObject(rhs)
,m_bIsOnce(rhs.m_bIsOnce)
,m_pTarget(rhs.m_pTarget)
{
}

CTriggerObj::~CTriggerObj()
{
}

HRESULT CTriggerObj::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);


	return S_OK;
}

_int CTriggerObj::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_TRIGGER);

	if (m_bIsUpdateDialogue && KEY_TAP(KEY::Z) && !m_bHasDialogue)
	{
		m_bHasDialogue = true;
	}

	return __super::Update_Object(fTimeDelta);
}

void CTriggerObj::LateUpdate_Object(void)
{
	if (m_pTarget && !m_pTarget->Is_Active())
		m_pTarget = nullptr;

	if (m_pLateEvent)
		m_pLateEvent();

	if (m_bHasDialogue && CUIMgr::GetInstance()->Get_Dialog()->Is_Active() && KEY_TAP(KEY::Z))
	{
		CUIMgr::GetInstance()->Get_Dialog()->Set_Active(false);
	}
}

void CTriggerObj::Render_Object(void)
{
}

void CTriggerObj::Event_Start(_uint iEventNum)
{


	auto src = m_iSubscribedEventNum.begin();
	auto tmp = m_pEventEvent.begin();

	for (size_t i = 0; i < m_iSubscribedEventNum.size(); i++)
	{
		if (*src == iEventNum)
		{
			(*tmp)();
			return;
		}

		src++;
		tmp++;
	}


}

void CTriggerObj::Event_End(_uint iEventNum)
{
	auto src = m_iSubscribedEventNum.begin();
	auto tmp = m_pEventEvent.begin();

	for (size_t i = 0; i < m_iSubscribedEventNum.size(); i++)
	{
		if (*src == iEventNum)
		{
			(*tmp)();
			return;
		}
		src++;
		tmp++;

	}

}

void CTriggerObj::Reset_Event()
{
}

void CTriggerObj::Free()
{
	__super::Free();
}

CTriggerObj* CTriggerObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos)
{
	CTriggerObj* ret = new CTriggerObj(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Trigger Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CTriggerObj::Set_Scale(const _vec3& pVec)
{
	m_pTransformCom->Set_Scale(pVec);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(pVec);
}

void CTriggerObj::Set_EventTrigger(const _uint& pEventNum, void(*pFunc)(), const _bool& pPushBack)
{
	Add_Subscribe(pEventNum, this);
	if (pPushBack)
	{
		m_iSubscribedEventNum.push_back(pEventNum);
		m_pEventEvent.push_back(pFunc);
	}
	else
	{
		m_iSubscribedEventNum.push_front(pEventNum);
		m_pEventEvent.push_front(pFunc);
	}
}

void CTriggerObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (!m_pTarget || m_pTarget == pCollider->GetOwner())
	{
		if (m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::ENTER] == nullptr)
			return;
		m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::ENTER]();
		if (m_bIsOnce)
			Set_Active(false);

	}
}

void CTriggerObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (!m_pTarget || m_pTarget == pCollider->GetOwner())
	{
		if (m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::STAY] == nullptr)
			return;

		m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::STAY]();

	}
}

void CTriggerObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (!m_pTarget || m_pTarget == pCollider->GetOwner())
	{
		if (m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::EXIT] == nullptr)
			return;

		m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::EXIT]();

		if (m_bHasDialogue)
			m_bHasDialogue = false;
	}

}
