#include "Catapult.h"
#include "Export_Function.h"
#include	"Player.h"
#include	"PushStone.h"

CCatapult::CCatapult(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev), m_pThrowingStone(nullptr), m_vStonePos(0, -0.3f, 0)
{
	m_tInfo.m_bIsPushable = true;
	m_tInfo.m_bIsBreakable = true;
}

CCatapult::CCatapult(const CCatapult& rhs) : CFieldObject(rhs), m_pThrowingStone(rhs.m_pThrowingStone), m_vStonePos(rhs.m_vStonePos)
{
}

CCatapult::~CCatapult()
{
}

HRESULT CCatapult::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Normal", L"Proto_Tex_Catapult_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"Throw", L"Proto_Tex_Catapult_Fire", 0.1f);

	m_pAnimator->Play_Animation(L"Normal", false);


	return S_OK;
}

_int CCatapult::Update_Object(const _float& fTimeDelta)
{


	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	if (m_pThrowingStone)
	{
		_vec3	src, tmp;
		_float dst;

		m_pThrowingStone->Get_TransformCom()->Get_Info(INFO_POS, &src);
		m_pTransformCom->Get_Info(INFO_POS, &tmp);
		tmp += m_vStonePos;
		if ((dst = D3DXVec3Length(&(tmp - src))) && dst > 1.f)
			m_pThrowingStone->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&_vec3(), &(tmp - src)), dst * 10, fTimeDelta);
		else if (dst > 0.01f)
			m_pThrowingStone->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&_vec3(), &(tmp - src)), 10, fTimeDelta);
		else
			dst = 0.f;

	}

	return __super::Update_Object(fTimeDelta);
}

void CCatapult::LateUpdate_Object(void)
{
}

void CCatapult::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CCatapult::Free()
{
	__super::Free();
}

CCatapult* CCatapult::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CCatapult* ret = new CCatapult(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Catapult create Failed");
		return nullptr;
	}
	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CCatapult::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{
	CPlayer* src;
	CPushStone*	tmp;
	
	if ((src = dynamic_cast<CPlayer*>(pCollisionObj)))
	{
		if ()
		{

		}
		else
		{

		}
	}
	else if (tmp = dynamic_cast<CPushStone*>(pCollisionObj))
	{
		m_pThrowingStone = tmp;
		m_pThrowingStone->Set_Active(false);
	}
}

void CCatapult::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CCatapult::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
{
}



void CCatapult::Event_Start(_uint iEventNum)
{
}

void CCatapult::Event_End(_uint iEventNum)
{
}

void CCatapult::Set_SubscribeEvent(_uint pEvent)
{
}
