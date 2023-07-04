#include "../Include/stdafx.h"
#include "Player_State_Walk.h"
#include "Component.h"
#include "Animator.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"

CPlayer_State_Walk::CPlayer_State_Walk(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
}

CPlayer_State_Walk::~CPlayer_State_Walk()
{
}

HRESULT CPlayer_State_Walk::Ready_State(void)
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
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Right", true);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_LeftDown", true);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_LeftUp", true);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_RightUp", true);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_RightDown", true);
		break;
	}
	return S_OK;
}

_int CPlayer_State_Walk::Update_State(const _float& fTimeDelta)
{
	dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 5.f, fTimeDelta);
	if (m_fAccTime > m_fKeyDelayTime)
	{
		Key_Input(fTimeDelta);
		m_fAccTime = 0.0f;
	}
	else
		m_fAccTime += fTimeDelta;


	return 0;
}

void CPlayer_State_Walk::LateUpdate_State(void)
{

}

void CPlayer_State_Walk::Render_State(void)
{

}

void CPlayer_State_Walk::Key_Input(const _float& fTimeDelta)
{	
	if (KEY_HOLD(KEY::SHIFT))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::RUN);
	}

	if (KEY_HOLD(KEY::SPACE))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_LU,L"Walk_LeftUp");
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_RU, L"Walk_RightUp");
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_RD, L"Walk_RightDown");
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::DOWN_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_LD, L"Walk_LeftDown");
	}
	else if (KEY_HOLD(KEY::UP_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_U, L"Walk_Up");
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_D, L"Walk_Down");
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_L, L"Walk_Left");
	}
	else if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		Change_Dir(OBJ_DIR::DIR_R, L"Walk_Right");
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}
	

	if (KEY_HOLD(KEY::X))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}
	if (KEY_HOLD(KEY::A))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}
	
}
