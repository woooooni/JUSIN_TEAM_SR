#include "../Include/stdafx.h"
#include "Player_State_Lift.h"
#include "Transform.h"
#include "Texture.h"
#include "Player.h"

CPlayer_State_Lift::CPlayer_State_Lift(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
}

CPlayer_State_Lift::~CPlayer_State_Lift()
{
}

HRESULT CPlayer_State_Lift::Ready_State(void)
{
	m_eLiftState = LIFT_STATE::LIFTUP;


	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Up");
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Down");
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Left");
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Right");
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_LeftDown");
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_LeftUp");
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_RightUp");
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_RightDown");
		break;
	}

	return S_OK;
}

_int CPlayer_State_Lift::Update_State(const _float& fTimeDelta)
{
	switch (m_eLiftState)
	{
	case LIFT_STATE::LIFTUP:
		Update_LiftUp(fTimeDelta);
		break;
	case LIFT_STATE::LIFTIDLE:
		Update_LiftIdle(fTimeDelta);
		break;
	case LIFT_STATE::LIFTWALK:
		Update_LiftWalk(fTimeDelta);
		break;
	case LIFT_STATE::LIFTDOWN:
		Update_LiftDown(fTimeDelta);
		break;
	}

	return 0;
}

void CPlayer_State_Lift::LateUpdate_State(void)
{
}

void CPlayer_State_Lift::Render_State(void)
{
}

_int CPlayer_State_Lift::Update_LiftUp(const _float& fTimeDelta)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		m_eLiftState = LIFT_STATE::LIFTIDLE;
		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Up");
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Down");
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Left");
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Right");
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftDown");
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftUp");
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightUp");
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightDown");
			break;
		}
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftIdle(const _float& fTimeDelta)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 ||
		GetAsyncKeyState(VK_RIGHT) & 0x8000 ||
		GetAsyncKeyState(VK_DOWN) & 0x8000 ||
		GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_eLiftState = LIFT_STATE::LIFTWALK;

		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Up");
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Down");
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Left");
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Right");
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_LeftDown");
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_LeftUp");
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_RightUp");
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_RightDown");
			break;
		}
	}

	if (GetAsyncKeyState('G') & 0x8000)
	{
		m_eLiftState = LIFT_STATE::LIFTDOWN;


		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Up");
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Down");
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Left");
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Right");
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftDown");
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftUp");
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightUp");
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightDown");
			break;
		}
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftDown(const _float& fTimeDelta)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftWalk(const _float& fTimeDelta)
{
	dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 2.5f, fTimeDelta);

	if (m_fAccTime > m_fKeyDelayTime)
	{
		m_fAccTime = 0.0f;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_LU, L"LiftWalk_LeftUp");
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_RU, L"LiftWalk_RightUp");
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_RD, L"LiftWalk_RightDown");
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_LD, L"LiftWalk_LeftDown");
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_U, L"LiftWalk_Up");
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_D, L"LiftWalk_Down");
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_L, L"LiftWalk_Left");
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Change_Dir(OBJ_DIR::DIR_R, L"LiftWalk_Right");
		}
		else
		{
			m_eLiftState = LIFT_STATE::LIFTIDLE;
			switch (m_pOwner->GetObj_Dir())
			{
			case OBJ_DIR::DIR_U:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Up");
				break;
			case OBJ_DIR::DIR_D:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Down");
				break;
			case OBJ_DIR::DIR_L:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Left");
				break;
			case OBJ_DIR::DIR_R:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Right");
				break;
			case OBJ_DIR::DIR_LD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftDown");
				break;
			case OBJ_DIR::DIR_LU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftUp");
				break;
			case OBJ_DIR::DIR_RU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightUp");
				break;
			case OBJ_DIR::DIR_RD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightDown");
				break;
			}
		}

		if (GetAsyncKeyState('G') & 0x8000)
		{
			m_eLiftState = LIFT_STATE::LIFTDOWN;


			switch (m_pOwner->GetObj_Dir())
			{
			case OBJ_DIR::DIR_U:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Up");
				break;
			case OBJ_DIR::DIR_D:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Down");
				break;
			case OBJ_DIR::DIR_L:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Left");
				break;
			case OBJ_DIR::DIR_R:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Right");
				break;
			case OBJ_DIR::DIR_LD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftDown");
				break;
			case OBJ_DIR::DIR_LU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftUp");
				break;
			case OBJ_DIR::DIR_RU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightUp");
				break;
			case OBJ_DIR::DIR_RD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightDown");
				break;
			}
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;
	}


	

	return 0;
}
