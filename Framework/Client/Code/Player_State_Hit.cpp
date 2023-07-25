#include "../Include/stdafx.h"
#include "Player_State_Hit.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "RigidBody.h"

CPlayer_State_Hit::CPlayer_State_Hit(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos[i].resize(3, { 0.0f,0.0f,0.0f });
	}
	Set_Hat();
}

CPlayer_State_Hit::~CPlayer_State_Hit()
{

}

HRESULT CPlayer_State_Hit::Ready_State(void)
{
	_vec3 vDir;
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Up", FALSE);
		vDir = { 0.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Down", FALSE);
		vDir = { 0.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Left", FALSE);
		vDir = { 1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Right", FALSE);
		vDir = { -1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_LeftDown", FALSE);
		vDir = { 1.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_LeftUp", FALSE);
		vDir = { 1.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_RightUp", FALSE);
		vDir = { -1.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_RightDown", FALSE);
		vDir = { -1.0f, 0.0f, 1.0f };
		break;
	}

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));

	D3DXVec3Normalize(&vDir, &vDir);

	m_pOwner->Get_RigidBodyCom()->AddForce(vDir * 80.0f);

	dynamic_cast<CPlayer*>(m_pOwner)->Minus_Hp(1);
	


	return S_OK;
}

_int CPlayer_State_Hit::Update_State(const _float& fTimeDelta)
{
	return 0;
}

void CPlayer_State_Hit::LateUpdate_State(void)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		
		if (dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerStat().iHp <= 0)
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::DOWN);
		}
		else
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
			dynamic_cast<CPlayer*>(m_pOwner)->Set_Invincible();
		}
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

}

void CPlayer_State_Hit::Render_State(void)
{
}

void CPlayer_State_Hit::Reset_State(void)
{
}

void CPlayer_State_Hit::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.005f;
	vPos += m_vecHatPos[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Hit::Set_Hat()
{


	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_D][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_L][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_R][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LU][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_LD][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RU][2] = { 0.0f, 0.4f, 0.0f };

	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][0] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][1] = { 0.0f, 0.4f, 0.0f };
	m_vecHatPos[(_uint)OBJ_DIR::DIR_RD][2] = { 0.0f, 0.4f, 0.0f };



	m_fAngle[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;

	m_fAngle[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;




	m_fScale[(_uint)OBJ_DIR::DIR_U][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_U][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_D][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_D][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_L][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_L][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_R][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_R][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LU][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LU][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_LD][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_LD][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RU][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RU][2] = 1.0f;

	m_fScale[(_uint)OBJ_DIR::DIR_RD][0] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][1] = 1.0f;
	m_fScale[(_uint)OBJ_DIR::DIR_RD][2] = 1.0f;

}
