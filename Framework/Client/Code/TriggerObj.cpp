#include "TriggerObj.h"
#include	"Export_Function.h"

CTriggerObj::CTriggerObj(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_INTERACTION, OBJ_ID::NEAR_REACT)
{
	for (_uint i = 0; i < (_uint)COLLIDE_EVENT_TYPE::TYPE_END; i++)
	{
		m_pCollideEvent[i] = nullptr;
	}
}

CTriggerObj::CTriggerObj(const CTriggerObj& rhs) : CGameObject(rhs)
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
	return __super::Update_Object(fTimeDelta);
}

void CTriggerObj::LateUpdate_Object(void)
{
}

void CTriggerObj::Render_Object(void)
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

void CTriggerObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::ENTER]();

}

void CTriggerObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::STAY]();
}

void CTriggerObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::EXIT]();

}
