#include "../Include/stdafx.h"
#include "Player_State_Jump.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "RigidBody.h"

CPlayer_State_Jump::CPlayer_State_Jump(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
}

CPlayer_State_Jump::~CPlayer_State_Jump()
{
}

HRESULT CPlayer_State_Jump::Ready_State(void)
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
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_Up", FALSE);
		m_vDir = { 0.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_Down", FALSE);
		m_vDir = { 0.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_Left", FALSE);
		m_vDir = { -1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_Right", FALSE);
		m_vDir = { 1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_LeftDown", FALSE);
		m_vDir = { -1.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_LeftUp", FALSE);
		m_vDir = { -1.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_RightUp", FALSE);
		m_vDir = { 1.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Jump_RightDown", FALSE);
		m_vDir = { 1.0f, 0.0f, -1.0f };
		break;
	}


	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bJump = false;

	return S_OK;
}

_int CPlayer_State_Jump::Update_State(const _float& fTimeDelta)
{
	if (!m_bJump && m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() == 4)
	{
		m_pOwner->Get_RigidBodyCom()->AddForce(m_vDir * 30.0f);
		m_pOwner->Get_RigidBodyCom()->AddForce(_vec3(0.0f, 120.0f, 0.0f));
		m_pOwner->Get_RigidBodyCom()->SetGround(false);
		m_bJump = true;
	}
	if (m_pOwner->Get_RigidBodyCom()->GetVelocity().y > 0.0f && m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 4)
	{
		m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(4);
	}
	if (m_pOwner->Get_RigidBodyCom()->GetVelocity().y < 0.0f && m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() < 6)
	{
		m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(6);
	}
	
	return 0;
}

void CPlayer_State_Jump::LateUpdate_State(void)
{
	if ( dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished() && m_pOwner->Get_RigidBodyCom()->IsGround())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}
}

void CPlayer_State_Jump::Render_State(void)
{
}
