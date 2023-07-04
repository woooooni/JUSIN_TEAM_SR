#include "../Include/stdafx.h"
#include "Player_State_Idle.h"

#include "Animator.h"
#include "Player.h"

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
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Up");
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Down");
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Left");
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Right");
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftDown");
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftUp");
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightUp");
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightDown");
		break;
	}
	return S_OK;
}

_int CPlayer_State_Idle::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Idle::LateUpdate_State(void)
{
}

void CPlayer_State_Idle::Render_State(void)
{
}

void CPlayer_State_Idle::Key_Input(const _float& fTimeDelta)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 || 
		GetAsyncKeyState(VK_RIGHT) & 0x8000 ||
		GetAsyncKeyState(VK_DOWN) & 0x8000 ||
		GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(OBJ_STATE::RUN);
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(OBJ_STATE::MOVE);
	}


	if (GetAsyncKeyState('X') & 0x8000)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(OBJ_STATE::ROLL);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(OBJ_STATE::JUMP);
	}
}
