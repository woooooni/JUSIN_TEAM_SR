#include "ClearBomb.h"
#include "Export_Function.h"
#include	"ClearField.h"
#include	"Pool.h"

void CClearBomb::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && m_pRigidBodyCom->IsGround())
	{
		_vec3 myPos;
		m_pTransformCom->Get_Info(INFO_POS, &myPos);
		Set_Active(false);
		CClearField* src = dynamic_cast<CClearField*>(CPool<CClearField>::Get_Obj());

		if (src)
		{
			src->Get_TransformCom()->Set_Pos(&myPos);
		}
		else
		{
			src = CClearField::Create(m_pGraphicDev, myPos);
		}
		NULL_CHECK(src);
		Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"ClearField", src);

		CPool<CClearBomb>::Return_Obj(this);

	}
}

void CClearBomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup != COLLISION_GROUP::COLLIDE_PLAYER && _eCollisionGroup != COLLISION_GROUP::COLLIDE_TRIGGER && _eCollisionGroup != COLLISION_GROUP::COLLIDE_MONSTER && _eCollisionGroup != COLLISION_GROUP::COLLIDE_BOSS)
		Push_Me(pCollider);
}

void CClearBomb::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CClearBomb::CClearBomb(LPDIRECT3DDEVICE9 pDev) : CFieldObject(pDev, OBJ_ID::CLEAR_BOMB)
{
	m_tInfo.m_bIsGrabbable = true;
	m_tInfo.m_bIsAttackable = true;
	m_tInfo.m_bIsPushable = true;
}

CClearBomb::CClearBomb(const CClearBomb& rhs) : CFieldObject(rhs)
{
}

CClearBomb::~CClearBomb()
{
}

HRESULT CClearBomb::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_ClearBomb", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);

	Set_MinHeight(0.5f);

	return S_OK;
}

_int CClearBomb::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);

	return __super::Update_Object(fTimeDelta);
}

void CClearBomb::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CClearBomb::Render_Object(void)
{
	__super::Render_Object();
}

void CClearBomb::Free()
{
	__super::Free();
}

CClearBomb* CClearBomb::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos)
{
	CClearBomb* ret = new CClearBomb(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("ClearBombCreat failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}
