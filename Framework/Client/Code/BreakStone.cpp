#include	"BreakStone.h"
#include	"Export_Function.h"
#include	"Pool.h"

CBreakStone::CBreakStone(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::PUSH_STONE) , m_eColor(JELLY_COLOR::JELLY_END), m_bBreak(false)
{
}

CBreakStone::CBreakStone(const CBreakStone& rhs) : CFieldObject(rhs), m_eColor(rhs.m_eColor), m_bBreak(rhs.m_bBreak)
{
}

CBreakStone::~CBreakStone()
{
}


HRESULT		CBreakStone::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Yellow", L"Proto_Tex_BreakStone_Yellow", 0.f);
	m_pAnimator->Add_Animation(L"Red", L"Proto_Tex_BreakStone_Red", 0.f);
	m_pAnimator->Add_Animation(L"Green", L"Proto_Tex_BreakStone_Green", 0.f);
	m_pAnimator->Add_Animation(L"Breaking", L"Proto_Tex_BreakStone_Breaking", 0.1f);

	return S_OK;
}


_int CBreakStone::Update_Object(const _float& fTimeDelta)
{
	if (m_bBreak && m_pAnimator->GetCurrAnimation()->Get_Idx() == (m_pAnimator->GetCurrAnimation()->Get_Size() - 1))
	{
		
	}
	else
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
		Add_RenderGroup(RENDER_ALPHA, this);
	}


	return __super::Update_Object(fTimeDelta);
}

void CBreakStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBreakStone::Render_Object(void)
{
	if (m_bBreak)
	{
		switch (m_eColor)
		{
		case JELLY_COLOR::YELLOW:
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 0));
			break;
		case JELLY_COLOR::GREEN:
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 0, 255, 0));
			break;
		case JELLY_COLOR::RED:
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0));
			break;

		default:
			break;
		}

	}
	__super::Render_Object();
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBreakStone::Add_Subscribe(_uint iNum)
{
	Engine::Add_Subscribe(iNum, this);
}

void CBreakStone::Free()
{
	__super::Free();
}

CBreakStone* CBreakStone::Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLOR pColor, const _uint& p_EventNum, const _vec3 p_Pos)
{
	if (pColor != JELLY_COLOR::RED && pColor != JELLY_COLOR::GREEN && pColor != JELLY_COLOR::YELLOW)
		return nullptr;

	CBreakStone* ret = new CBreakStone(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Breakstone creata failed");
		return nullptr;
	}

	ret->m_eColor = pColor;

	switch (ret->m_eColor)
	{	
	case JELLY_COLOR::YELLOW:
		ret->m_pAnimator->Play_Animation(L"Yellow", false);
		break;
	case JELLY_COLOR::GREEN:
		ret->m_pAnimator->Play_Animation(L"Green", false);
		break;
	case JELLY_COLOR::RED:
		ret->m_pAnimator->Play_Animation(L"Red", false);
		break;

	default:
		break;
	}

	ret->Add_Subscribe(p_EventNum);
	ret->Get_TransformCom()->Set_Pos(&p_Pos);
	ret->Set_MinHeight(0.5f);
	return ret;
}

void CBreakStone::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBreakStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBreakStone::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBreakStone::Event_Start(_uint iEventNum)
{
	m_bBreak = true;
	m_pAnimator->Play_Animation(L"Breaking", false);
}

void CBreakStone::Event_End(_uint iEventNum)
{
}
