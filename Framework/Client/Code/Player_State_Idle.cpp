#include "../Include/stdafx.h"
#include "Player_State_Idle.h"

#include "Animator.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Transform.h"
#include "Player_State_Skill.h"

CPlayer_State_Idle::CPlayer_State_Idle(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
}

CPlayer_State_Idle::~CPlayer_State_Idle()
{
}

HRESULT CPlayer_State_Idle::Ready_State(void)
{
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Right", true);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftDown", true);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftUp", true);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightUp", true);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightDown", true);
		break;
	}

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));

	return S_OK;

	
}

_int CPlayer_State_Idle::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Idle::LateUpdate_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::GETTIEM);
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_BalloonFly())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::BALLOONFLY);
	}
}

void CPlayer_State_Idle::Render_State(void)
{
}

void CPlayer_State_Idle::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::LEFT_ARROW) || 
		KEY_HOLD(KEY::RIGHT_ARROW) ||
		KEY_HOLD(KEY::UP_ARROW) ||
		KEY_HOLD(KEY::DOWN_ARROW))
	{
		if (KEY_HOLD(KEY::SHIFT))
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::RUN);
		else if (dynamic_cast<CPlayer*>(m_pOwner)->Is_Push())
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::PUSH);
		}
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
	}


	if (KEY_TAP(KEY::X))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (KEY_HOLD(KEY::A))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}

	if (KEY_HOLD(KEY::T))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::HIT);
	}

	if (KEY_HOLD(KEY::G))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::LIFT);
	}

	if (KEY_HOLD(KEY::D))
	{
		//dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::DOWN);
	}

	if (KEY_TAP(KEY::R))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::PUSH);
	}


	if (KEY_TAP(KEY::Q))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Set_GetItem(true);
	}


	if (KEY_TAP(KEY::W))
	{
		//dynamic_cast<CPlayer*>(m_pOwner)->Set_BalloonFly(true);
	}

	if (KEY_TAP(KEY::E))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::DRAWING);
	}

	if (KEY_TAP(KEY::K))
	{
		if(dynamic_cast<CPlayer*>(m_pOwner)->Is_HaveSkill())
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SKILL);
	}

	if (KEY_TAP(KEY::J))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Add_Skill(PLAYER_SKILL::GOLEMFIST);
		dynamic_cast<CPlayer*>(m_pOwner)->Set_Skill(PLAYER_SKILL::GOLEMFIST);
	}
}
