#include "../Include/stdafx.h"
#include "Player_State_Run.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Texture.h"

CPlayer_State_Run::CPlayer_State_Run(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
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
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_Right", true);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftDown", true);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_LeftUp", true);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightUp", true);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Run_RightDown", true);
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
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

}

void CPlayer_State_Run::Render_State(void)
{
}

void CPlayer_State_Run::Reset_State(void)
{
}

void CPlayer_State_Run::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_LU, L"Run_LeftUp");
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_RU, L"Run_RightUp");
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_RD, L"Run_RightDown");
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_LD, L"Run_LeftDown");
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_U, L"Run_Up");
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_D, L"Run_Down");
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_L, L"Run_Left");
	}
	else if (KEY_HOLD(KEY::RIGHT_ARROW) && KEY_HOLD(KEY::SHIFT))
	{
		Change_Dir(OBJ_DIR::DIR_R, L"Run_Right");
	}
	else
	{
		if (KEY_HOLD(KEY::UP_ARROW) ||
			KEY_HOLD(KEY::RIGHT_ARROW) ||
			KEY_HOLD(KEY::DOWN_ARROW) ||
			KEY_HOLD(KEY::LEFT_ARROW))
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		
	}


	if (KEY_HOLD(KEY::X))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}

	if (KEY_HOLD(KEY::SPACE))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (KEY_HOLD(KEY::A))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}
}

void CPlayer_State_Run::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(m_fScale[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);

}

void CPlayer_State_Run::Set_Hat()
{
	//포지션
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5] = { 0.01f, 0.013f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7] = { -0.01f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, 0.0f, 0.0f};
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.0f, 0.0f};

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { -0.1f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { -0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { -0.14f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3] = { -0.15f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4] = { -0.15f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5] = { -0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6] = { -0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7] = { -0.15f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { 0.1f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.14f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { 0.15f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { 0.15f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { 0.08f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { 0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { 0.15f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0] = { -0.03f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2] = { -0.03f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][3] = { -0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][4] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][5] = { -0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][6] = { -0.03f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { -0.04f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { -0.09f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { -0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3] = { -0.09f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5] = { -0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6] = { -0.16f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7] = { -0.1f, 0.075f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { 0.04f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { 0.09f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { 0.09f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { 0.12f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.16f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { 0.1f, 0.075f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { 0.0f, 0.0f, 0.0f    };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { 0.0f, 0.0f, 0.0f    };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0] = { 0.03f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { 0.03f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][3] = { 0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][4] = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][5] = { 0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][6] = { 0.03f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][9] = { 0.0f, 0.0f, 0.0f };



	//각도
	m_fAngle[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][7] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][6] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LD][0] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][1] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][2] = 5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][3] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][4] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][5] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][6] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][6] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RD][0] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][1] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][2] = -5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][3] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][4] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][5] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][6] = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;


	//스케일
	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6] = 0.92f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7] = 0.92f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.6f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5] = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.70f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;
}
