#include "../Include/stdafx.h"
#include "BenchObj.h"
#include	"Export_Function.h"

CBenchObj::CBenchObj(LPDIRECT3DDEVICE9 pGra) : CFieldObject(pGra, OBJ_ID::BENCH)
{

}

CBenchObj::CBenchObj(const CBenchObj& rhs) : CFieldObject(rhs)
{
}

CBenchObj::~CBenchObj()
{
}

HRESULT CBenchObj::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	CComponent* pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	Set_MinHeight(0.5f);


	return S_OK;
}

_int CBenchObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

	return __super::Update_Object(fTimeDelta);
}

void CBenchObj::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBenchObj::Render_Object(void)
{
	__super::Render_Object();
}

void CBenchObj::Add_Subscribe(_uint iNum)
{
}

void CBenchObj::Free()
{
	__super::Free();
}

CBenchObj* CBenchObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CBenchObj* ret = new CBenchObj(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Bench Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CBenchObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBenchObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBenchObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBenchObj::Event_Start(_uint iEventNum)
{
}

void CBenchObj::Event_End(_uint iEventNum)
{
}
