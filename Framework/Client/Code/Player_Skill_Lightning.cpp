#include "Player_Skill_Lightning.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"

#include "Player_Skill_Aim.h"
#include "Player_Bullet_Lightning.h"
#include "Export_Function.h"


CPlayer_Skill_Lightning::CPlayer_Skill_Lightning(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAimMoveSpeed(0.1f)
{
}

CPlayer_Skill_Lightning::~CPlayer_Skill_Lightning()
{
}

HRESULT CPlayer_Skill_Lightning::Ready_State(void)
{

	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Up", true);
		m_vDir = { 0.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Down", true);
		m_vDir = { 0.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Left", true);
		m_vDir = { -1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Right", true);
		m_vDir = { 1.0f,0.0f,0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftDown", true);
		m_vDir = { -1.0f,0.0f,-1.0f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftUp", true);
		m_vDir = { -1.0f,0.0f,1.0f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightUp", true);
		m_vDir = { 1.0f,0.0f,1.0f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightDown", true);
		m_vDir = { 1.0f,0.0f,-1.0f };
		break;
	}
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir *= 3.0f;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
	m_vPos.y -= m_pOwner->Get_MinHeight() - 0.001f;

	m_bSkillStart = false;

	dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Set_Active(true);

	_vec3 vAimPos = m_vPos + m_vDir;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Get_TransformCom()->Set_Pos(&vAimPos);


	return S_OK;
}

_int CPlayer_Skill_Lightning::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	return 0;
}

void CPlayer_Skill_Lightning::LateUpdate_State(void)
{

}

void CPlayer_Skill_Lightning::Render_State(void)
{
}

void CPlayer_Skill_Lightning::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::K))
	{
		if (KEY_HOLD(KEY::LEFT_ARROW))
		{
			_matrix matRot;
			_vec3 vAxis = { 0.0f, 1.0f, 0.0f };
			D3DXMatrixRotationAxis(&matRot, &vAxis, -m_fAimMoveSpeed);
			D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
		}
		if (KEY_HOLD(KEY::RIGHT_ARROW))
		{
			_matrix matRot;
			_vec3 vAxis = { 0.0f, 1.0f, 0.0f };
			D3DXMatrixRotationAxis(&matRot, &vAxis, m_fAimMoveSpeed);
			D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
		}


		_vec3 vAimPos = m_vPos + m_vDir;
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Get_TransformCom()->Set_Pos(&vAimPos);

	}
	else if (KEY_AWAY(KEY::K))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);

		Shoot();

		dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Set_Active(false);


		
	}
}

HRESULT CPlayer_Skill_Lightning::Shoot(void)
{
	CGameObject* pLightning = nullptr;

	if (!pLightning)
	{
		pLightning = CPlayer_Bullet_Lightning::Create(Engine::Get_Device());
		NULL_CHECK_RETURN(pLightning, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Get_Layer(LAYER_TYPE::PLAYER)->Add_GameObject(L"Lightning", pLightning), E_FAIL);
		pLightning->Set_Active(true);
	}

	D3DXVec3Normalize(&m_vDir, &m_vDir);
	_vec3 vAimPos = m_vPos + m_vDir;
	vAimPos.y += 2.5f;
	dynamic_cast<CPlayer_Bullet_Lightning*>(pLightning)->Shoot(vAimPos,m_vDir, 5, 0);

	return 0;
}
