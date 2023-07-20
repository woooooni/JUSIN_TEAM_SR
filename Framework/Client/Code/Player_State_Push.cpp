#include "../Include/stdafx.h"
#include "Player_State_Push.h"
#include "Transform.h"
#include "Texture.h"
#include "Player.h"
#include "KeyMgr.h"
#include "RigidBody.h"

CPlayer_State_Push::CPlayer_State_Push(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
}

CPlayer_State_Push::~CPlayer_State_Push()
{
}

HRESULT CPlayer_State_Push::Ready_State(void)
{
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Push_Right", true);
		break;
	}

	m_eStartDir = m_pOwner->GetObj_Dir();
	dynamic_cast<CPlayer*>(m_pOwner)->Set_Push(false);
	return S_OK;
}

_int CPlayer_State_Push::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Push::LateUpdate_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();
}

void CPlayer_State_Push::Render_State(void)
{

}

void CPlayer_State_Push::Reset_State(void)
{
	CGameObject* pLiftObj = dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj();

	if (pLiftObj)
	{
		_vec3 vPos;
		pLiftObj->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

		if (vPos.y > pLiftObj->Get_MinHeight())
		{
			pLiftObj->Get_RigidBodyCom()->SetGround(false);
		}
	}
}

void CPlayer_State_Push::Key_Input(const _float& fTimeDelta)
{
	OBJ_DIR eInputDir = OBJ_DIR::DIR_END;

	if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_LU;
	}
	else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_RU;
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_RD;
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW) && KEY_HOLD(KEY::DOWN_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_LD;
	}
	else if (KEY_HOLD(KEY::UP_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_U;
	}
	else if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_D;
	}
	else if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_L;
	}
	else if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		eInputDir = OBJ_DIR::DIR_R;
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}

	if (m_eStartDir != eInputDir && eInputDir != OBJ_DIR::DIR_END)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
		m_pOwner->SetObj_Dir(eInputDir);
	}
	else
		dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 5.f, fTimeDelta);

}

void CPlayer_State_Push::Update_Hat()
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

void CPlayer_State_Push::Set_Hat()
{
	//포지션
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { 0.01f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][4] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][5] = { -0.01f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][6] = { -0.01f, 0.07f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.06f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][8] = { 0.01f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.03f, -0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][3] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][4] = { -0.02f, -0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][5] = { -0.02f, -0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][6] = { -0.02f, -0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][7] = { -0.02f, -0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, -0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { 0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { -0.05f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][3] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][4] = { -0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][5] = { -0.02f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][6] = { -0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][7] = { -0.05f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][8] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][9] = { -0.05f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { -0.02f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.05f, 0.02f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][3] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][4] = { 0.04f, 0.01f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][5] = { 0.02f, 0.03f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][6] = { 0.03f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][7] = { 0.05f, 0.04f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][8] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][9] = { 0.05f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0] = { 0.07f, -0.14f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1] = { -0.02f, -0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2] = { 0.01f, -0.21f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][3] = { -0.03f, -0.26f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][4] = { -0.09f, -0.16f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][5] = { -0.07f, -0.14f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][6] = { 0.03f, -0.14f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][7] = { 0.04f, -0.16f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][8] = { 0.03f, -0.17f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][9] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { 0.06f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { 0.07f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { 0.0f, -0.15f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][3] = { 0.01f, -0.15f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][4] = { -0.05f, -0.15f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][5] = { 0.05f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][6] = { -0.05f, -0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][7] = { -0.1f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][8] = { -0.05f, -0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { -0.1f, -0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { -0.05f, -0.08f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.07f, -0.11f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][3] = { 0.14f, -0.10f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][4] = { -0.01f, -0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][5] = { 0.01f, -0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][6] = { 0.07f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][7] = { 0.09f, -0.1f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][8] = { 0.02f, -0.05f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][9] = { 0.0f, 0.0f, 0.0f };


	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0] = { -0.1f, -0.12f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { -0.25f, -0.09f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { -0.17f, -0.22f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][3] = { -0.08f, -0.23f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][4] = { -0.1f, -0.19f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][5] = { -0.1f, -0.22f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][6] = { -0.05f, -0.19f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][7] = { -0.04f, -0.14f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][8] = { -0.08f, -0.12f, 0.0f };
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
	m_fAngle[(_uint)OBJ_DIR::DIR_L][4] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][4] = 0.0f;
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
	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][3] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][4] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][5] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][6] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][7] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][8] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][3] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][4] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][5] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][6] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][7] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][8] = 1.1f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][3] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][5] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][6] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][7] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][8] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][9] = 0.8f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 0.85f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][3] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][4] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][5] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][6] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][7] = 0.75f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][8] = 0.8f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][9] = 0.8f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][4] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][5] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][4] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][5] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;

}
