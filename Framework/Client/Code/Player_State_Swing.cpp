#include "../Include/stdafx.h"
#include "Player_State_Swing.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "Collider.h"

CPlayer_State_Swing::CPlayer_State_Swing(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Swing::~CPlayer_State_Swing()
{
}

HRESULT CPlayer_State_Swing::Ready_State(void)
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
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_Up", FALSE);
		m_vSwingDir = { 0.0f, 0.0f, 0.5f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_Down", FALSE);
		m_vSwingDir = { 0.0f, 0.0f, -0.5f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_Left", FALSE);
		m_vSwingDir = { -0.5f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_Right", FALSE);
		m_vSwingDir = { 0.5f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_LeftDown", FALSE);
		m_vSwingDir = { -0.5f, 0.0f, -0.5f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_LeftUp", FALSE);
		m_vSwingDir = { -0.5f, 0.0f, 0.5f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_RightUp", FALSE);
		m_vSwingDir = { 0.5f, 0.0f, 0.5f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Swing_RightDown", FALSE);
		m_vSwingDir = { 0.5f, 0.0f, -0.5f };
		break;
	}

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_ATTACK)->Set_Offset(m_vSwingDir);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_ATTACK)->Set_Active(true);
	return S_OK;
}

_int CPlayer_State_Swing::Update_State(const _float& fTimeDelta)
{
	return 0;
}

void CPlayer_State_Swing::LateUpdate_State(void)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_ATTACK)->Set_Offset(_vec3(0.0f,0.0f,0.0f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_ATTACK)->Set_Active(false);
	}
}

void CPlayer_State_Swing::Render_State(void)
{
}
