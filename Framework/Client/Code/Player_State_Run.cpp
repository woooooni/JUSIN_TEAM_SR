#include "../Include/stdafx.h"
#include "Player_State_Run.h"
#include "Transform.h"
#include "Player.h"

CPlayer_State_Run::CPlayer_State_Run(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
}

CPlayer_State_Run::~CPlayer_State_Run()
{
}

HRESULT CPlayer_State_Run::Ready_State(void)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_LU);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_RU);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_RD);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_LD);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_U);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_L);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_R);
	}

	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Up");
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Down");
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Left");
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Right");
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftDown");
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftUp");
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightUp");
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightDown");
		break;
	}
	return S_OK;
}

_int CPlayer_State_Run::Update_State(const _float& fTimeDelta)
{
	dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 10.f, fTimeDelta);
	if (m_fAccTime > m_fKeyDelayTime)
	{
		Key_Input(fTimeDelta);
		m_fAccTime = 0.0f;
	}
	else
		m_fAccTime += fTimeDelta;

	return 0;
}

void CPlayer_State_Run::LateUpdate_State(void)
{
}

void CPlayer_State_Run::Render_State(void)
{
}

void CPlayer_State_Run::Key_Input(const _float& fTimeDelta)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_LU, L"Run_LeftUp");
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_RU, L"Run_RightUp");
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_RD, L"Run_RightDown");
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_LD, L"Run_LeftDown");
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_U, L"Run_Up");
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_D, L"Run_Down");
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_L, L"Run_Left");
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_R, L"Run_Right");
	}
	else
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000 ||
			GetAsyncKeyState(VK_RIGHT) & 0x8000 ||
			GetAsyncKeyState(VK_DOWN) & 0x8000 ||
			GetAsyncKeyState(VK_LEFT) & 0x8000)
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		
	}


	if (GetAsyncKeyState('X') & 0x8000)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}
}
