#include "../Include/stdafx.h"
#include "Player_State_Jump.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "RigidBody.h"

CPlayer_State_Jump::CPlayer_State_Jump(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(12, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
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

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();
}

void CPlayer_State_Jump::Render_State(void)
{
}

void CPlayer_State_Jump::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Jump::Set_Hat()
{
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1]  = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2]  = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3]  = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4]  = { 0.0f, 0.13f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5]  = { 0.0f, 0.13f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6]  = { 0.0f, 0.14f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7]  = { 0.0f, 0.13f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8]  = { 0.0f, 0.13f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9]  = { 0.0f, 0.13f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][10] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][11] = { 0.0f, 0.08f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2]  = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3]  = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4]  = { 0.0f, 0.18f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5]  = { 0.0f, 0.18f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6]  = { 0.0f, 0.16f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7]  = { 0.0f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8]  = { 0.0f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9]  = { 0.0f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][10] = { -0.01f, -0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][11] = { 0.0f, 0.05f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2]  = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3]  = { -0.02f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4]  = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5]  = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6]  = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7]  = { -0.05f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8]  = { -0.05f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9]  = { -0.06f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][10] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][11] = { 0.01f, 0.02f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0]  = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { 0.02f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { 0.05f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][8] = { 0.05f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][9] = { 0.06f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][10] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][11] = { -0.01f, 0.02f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0]  = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1]  = { 0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2]  = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3]  = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4]  = { -0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5]  = { -0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6]  = { 0.0f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7]  = { 0.03f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8]  = { 0.03f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9]  = { 0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][10] = { 0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][11] = { 0.01f, 0.04f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0]  = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1]  = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2]  = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][3]  = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][4]  = { 0.07f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][5]  = { 0.07f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][6]  = { 0.01f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][7]  = { 0.06f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][8]  = { 0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][9]  = { 0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][10] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][11] = { 0.0f, 0.03f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0]  = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { -0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { 0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { 0.06f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.0f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { -0.03f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { -0.03f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { -0.02f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][10] = { -0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][11] = { -0.01f, 0.04f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0]  = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][3] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][4] = { -0.07f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][5] = { -0.07f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][6] = { -0.01f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][7] = { -0.06f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][8] = { -0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][9] = { -0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][10] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][11] = { 0.0f, 0.03f, 0.0f };

	



	//각도
	m_fAngle[(_uint)OBJ_DIR::DIR_U][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][4]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][5]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][6]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][8]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][9]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_D][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][4]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][5]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][6]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][8]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][9]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_L][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4]  = -30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5]  = -30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6]  = -25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9]  = 20.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = 30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][5] = 30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][6] = 25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][9] = -20.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LU][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][4]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][5]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][6]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][8]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][9]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LD][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][4]  = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][5]  = -15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][6]  = -10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][8]  = -5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][9]  = -5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RU][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][1]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][2]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][3]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][4]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][5]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][6]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][7]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][8]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][9]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][11] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RD][0]  = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][4] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][5] = 15.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][6] = 10.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][8] = 5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][9] = 5.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][10] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][11] = 0.0f;

	


	//스케일
	m_fScale[(_uint)OBJ_DIR::DIR_U][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4]  = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5]  = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6]  = 0.92f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9]  = 0.94f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][11] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1]  = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4]  = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5]  = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9]  = 0.94f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][11] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4]  = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5]  = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6]  = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7]  = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8]  = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][10] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][11] = 0.95f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.80f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][10] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][11] = 0.95f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5]  = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6]  = 0.93f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][11] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6]  = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9]  = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][11] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.93f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][11] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0]  = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.9f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][10] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][11] = 1.0f;

}
