#include "HoleObj.h"
#include	"Export_Function.h"
#include	"PushStone.h"
#include	"Pool.h"

CHoleObj::CHoleObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::BALPAN_OBJ), m_bIn(false)
{
}

CHoleObj::CHoleObj(const CHoleObj& rhs) : CFieldObject(rhs), m_bIn(rhs.m_bIn)
{
}

CHoleObj::~CHoleObj()
{
}

HRESULT CHoleObj::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_Hole", 0.f);
	m_pAnimator->Add_Animation(L"In", L"Proto_Tex_Hole_In", 0.1f);

	m_pAnimator->Play_Animation(L"Idle", false);

	return S_OK;
}

_int CHoleObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	if (!m_bIn)
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
	}

	return __super::Update_Object(fTimeDelta);
}

void CHoleObj::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CHoleObj::Render_Object(void)
{
	__super::Render_Object();
}

void CHoleObj::Free()
{
	__super::Free();
}

CHoleObj* CHoleObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CHoleObj* ret = new CHoleObj(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("holeobj create failed");
		return nullptr;

	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);
	ret->Set_MinHeight(0.003f);
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	return ret;
}

void CHoleObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	CPushStone* tmp;
	if (tmp = dynamic_cast<CPushStone*>(pCollider->GetOwner()))
	{
		m_bIn = true;
		CPool<CPushStone>::Return_Obj(tmp);
		m_pAnimator->Play_Animation(L"In", false);
		
	}
}

void CHoleObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CHoleObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
