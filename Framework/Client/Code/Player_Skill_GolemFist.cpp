#include "Player_Skill_GolemFist.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Player_Skill_Range.h"
#include "Player_Skill_Aim.h"
#include "Player_Bullet_GolemFist.h"
#include "Export_Function.h"

CPlayer_Skill_GolemFist::CPlayer_Skill_GolemFist(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_vPlusScale(_vec3(0.1f,0.1f,0.0f)), m_fAimMoveSpeed(5.0f)
{
	
}

CPlayer_Skill_GolemFist::~CPlayer_Skill_GolemFist()
{

}

HRESULT CPlayer_Skill_GolemFist::Ready_State(void)
{
	if (!m_pGolemFist)
	{
		CPlayer_Bullet_GolemFist* pGolemFist = CPlayer_Bullet_GolemFist::Create(Engine::Get_Device());
		Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Player_Skill_GolemFist", pGolemFist);
		m_pGolemFist = pGolemFist;
		dynamic_cast<CBullet*>(m_pGolemFist)->Set_Owner(m_pOwner);
	}


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
	m_vDir *= 1.5f;
	m_vDir.y += 0.0001f;

	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	CGameObject* pRange = dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange();
	pRange->Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"SkillRange", pRange);


	m_vPos.y -= m_pOwner->Get_MinHeight() - 0.001f;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange()->Get_TransformCom()->Set_Pos(&m_vPos);

	m_vScale = { 1.0f,1.0f,1.0f };
	dynamic_cast<CPlayer_Skill_Range*>(dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange())->Set_Range(m_vScale, m_vPos);
	m_bSkillStart = false;

	CGameObject* pAim = dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim();
	pAim->Set_Active(true);
	pAim->Get_TransformCom()->Set_Pos(&(m_vPos + m_vDir));
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"SkillAim", pAim);

	m_fRotAngle = 0.0f;

	
	return S_OK;
}

_int CPlayer_Skill_GolemFist::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	return 0;
}

void CPlayer_Skill_GolemFist::LateUpdate_State(void)
{

}

void CPlayer_Skill_GolemFist::Render_State(void)
{
}

void CPlayer_Skill_GolemFist::Key_Input(const _float& fTimeDelta)
{
	
	if (KEY_HOLD(KEY::K))
	{
		if (KEY_HOLD(KEY::LEFT_ARROW))
		{
			m_vDir.x += -m_fAimMoveSpeed * fTimeDelta;
		}
		if (KEY_HOLD(KEY::RIGHT_ARROW))
		{
			m_vDir.x += m_fAimMoveSpeed * fTimeDelta;
		}
		if (KEY_HOLD(KEY::UP_ARROW))
		{
			m_vDir.z += m_fAimMoveSpeed * fTimeDelta;
		}
		if (KEY_HOLD(KEY::DOWN_ARROW))
		{
			m_vDir.z += -m_fAimMoveSpeed * fTimeDelta;
		}
		

		if (D3DXVec3Length(&m_vDir) > 5.0f)
		{
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			m_vDir *= 5.0f;
		}

		dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Get_TransformCom()->Set_Pos(&(m_vPos + m_vDir));


		if (m_vScale.x < 10.0f)
		{
			m_vScale += m_vPlusScale * 4.0f;
			dynamic_cast<CPlayer_Skill_Range*>(dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange())->Set_Range(m_vScale, m_vPos);
			dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange()->Get_TransformCom()->RotationAxis(_vec3(0.0f, 1.0f, 0.0f), m_fRotAngle);
			m_fRotAngle += 0.07f;
		}
	}
	else if (KEY_AWAY(KEY::K))
	{
		dynamic_cast<CPlayer_Skill_Range*>(dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange())->Reset_Range();
		dynamic_cast<CPlayer*>(m_pOwner)->Get_SkillRange()->Set_Active(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);

		dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Set_Active(false);


		Shoot();
	}
}

void CPlayer_Skill_GolemFist::Shoot(void)
{
	m_vDir.y += 10.0f;
	m_pGolemFist->Get_TransformCom()->Set_Pos(&(m_vPos + m_vDir));
	m_pGolemFist->Get_RigidBodyCom()->SetGround(false);
	m_pGolemFist->Get_RigidBodyCom()->AddForce(_vec3(0.0f, -10.0f, 0.0f));
	m_pGolemFist->Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"SkillGolemFist", m_pGolemFist);
}
