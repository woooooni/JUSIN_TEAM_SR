#include "../Include/stdafx.h"
#include "Player_State_Swing.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "Collider.h"

CPlayer_State_Swing::CPlayer_State_Swing(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
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

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();
}

void CPlayer_State_Swing::Render_State(void)
{
}

void CPlayer_State_Swing::Update_Hat()
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

void CPlayer_State_Swing::Set_Hat()
{
	//포지션
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5] = { 0.0f, 0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5] = { 0.0f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { 0.01f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { -0.13f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4] = { -0.05f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { -0.01f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.13f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { 0.05f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { 0.0f, 0.0f, 0.0f };
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

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { 0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { -0.05f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5] = { 0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { -0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { 0.05f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { -0.03f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { 0.0f, 0.0f, 0.0f };


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
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1] = 40.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = -40.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;


	//스케일
	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 0.65f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;
}
