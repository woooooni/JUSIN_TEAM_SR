#include "../Include/stdafx.h"
#include "Player_State_Walk.h"
#include "Component.h"
#include "Animator.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Texture.h"
#include		"Export_Function.h"

CPlayer_State_Walk::CPlayer_State_Walk(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
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

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));


	m_fAccTime = 0.0f;
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


	if(dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();


	if(dynamic_cast<CPlayer*>(m_pOwner)->Is_Push())
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::PUSH);

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::GETTIEM);
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_BalloonFly())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::BALLOONFLY);
	}
}

void CPlayer_State_Walk::Render_State(void)
{

}

void CPlayer_State_Walk::Reset_State(void)
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

void CPlayer_State_Walk::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0005f;
	vPos += m_vecHatPos[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(m_fScale[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);

}

void CPlayer_State_Walk::Set_Hat()
{
	//포지션
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3] = { -0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4] = { 0.0f, 0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9] = { 0.01f, 0.09f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.02f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3] = { -0.01f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5] = { -0.01f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7] = { -0.02f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, 0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.08f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { 0.10f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { 0.06f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { 0.01f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4] = { 0.09f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5] = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7] = { 0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8] = { 0.06f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9] = { 0.09f, 0.02f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { -0.10f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { -0.06f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { -0.01f, 0.00f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { -0.03f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { -0.09f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { -0.03f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { -0.06f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][8] = { -0.06f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][9] = { -0.09f, 0.02f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0] = { -0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2] = { 0.035f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][3] = { 0.04f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][4] = { 0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][5] = { 0.02f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][6] = { -0.01f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][7] = { -0.01f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][8] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][9] = { -0.01f, 0.02f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { -0.01f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { -0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { -0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3] = { -0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4] = { -0.01f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5] = { -0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6] = { -0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7] = { 0.02f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9] = { -0.01f, 0.04f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { 0.01f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { 0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { 0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { 0.01f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { 0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.01f, 0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { -0.02f, 0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { 0.01f, 0.04f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0] = { 0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { -0.035f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][3] = { -0.04f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][4] = { -0.05f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][5] = { -0.02f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][6] = { 0.01f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][7] = { 0.01f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][8] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][9] = { 0.01f, 0.02f, 0.0f };

	

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

	m_fAngle[(_uint)OBJ_DIR::DIR_L][0] = -35.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1] = -30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2] = -25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][3] = -35.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4] = -25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5] = -20.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6] = -30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7] = -25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8] = -25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9] = -30.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 35.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 35.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = 25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][5] = 20.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][6] = 30.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][7] = 25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][8] = 25.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][9] = 30.0f;

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
	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9] = 0.98f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9] = 0.98f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7] = 0.73f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9] = 0.7f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.73f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.7f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.7f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9] = 0.98f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9] = 0.98f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.91f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.98f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.98f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.90f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.95f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.98f;
}
