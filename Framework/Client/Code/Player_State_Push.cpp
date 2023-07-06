#include "../Include/stdafx.h"
#include "Player_State_Push.h"
#include "Transform.h"
#include "Texture.h"
#include "Player.h"
#include "KeyMgr.h"

CPlayer_State_Push::CPlayer_State_Push(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
}

CPlayer_State_Push::~CPlayer_State_Push()
{
}

HRESULT CPlayer_State_Push::Ready_State(void)
{
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Right", true);
		break;
	}

	m_eStartDir = m_pOwner->GetObj_Dir();
	dynamic_cast<CPlayer*>(m_pOwner)->Set_Push(false);
	return S_OK;
}

_int CPlayer_State_Push::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Push::LateUpdate_State(void)
{

}

void CPlayer_State_Push::Render_State(void)
{
}

void CPlayer_State_Push::Key_Input(const _float& fTimeDelta)
{
	OBJ_DIR eInputDir = OBJ_DIR::DIR_END;

	if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_LU;
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_RU;
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_RD;
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::DOWN_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_LD;
	}
	else if (KEY_HOLD(KEY::UP_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_U;
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_D;
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_L;
	}
	else if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_R;
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}

	if(m_eStartDir != eInputDir)
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
	else
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 5.f, fTimeDelta);

}
