#include "../Include/stdafx.h"
#include "Player_State_Walk.h"
#include "Component.h"
#include "Animator.h"
#include "Transform.h"
#include "Player.h"

CPlayer_State_Walk::CPlayer_State_Walk(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Walk::~CPlayer_State_Walk()
{
}

HRESULT CPlayer_State_Walk::Ready_State(void)
{	
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Up");
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Down");
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Left");
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_Right");
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_LeftDown");
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_LeftUp");
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_RightUp");
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Walk_RightDown");
		break;
	}
	return S_OK;
}

_int CPlayer_State_Walk::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
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
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_LU,L"Walk_LeftUp");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_LU, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_RU, L"Walk_RightUp");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_RU, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_RD, L"Walk_RightDown");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_RD, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_LD, L"Walk_LeftDown");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_LD, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_U, L"Walk_Up");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_U, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_D, L"Walk_Down");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_D, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_L, L"Walk_Left");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_L, 10.f, fTimeDelta);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		Change_Dir(OBJ_DIR::DIR_R, L"Walk_Right");
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(OBJ_DIR::DIR_R, 10.f, fTimeDelta);
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(OBJ_STATE::IDLE);
	}
	
}
