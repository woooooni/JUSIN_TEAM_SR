#include "../Header/Player_State_Skill.h"
#include "Engine_Enum.h"
#include "Player_State.h"
#include "Player_Skill_GolemFist.h"
#include "Player_Skill_Turtle.h"
#include "Player_Skill_Drill.h"
#include "Player_Skill_Lightning.h"
#include "Player_Skill_Missile.h"

CPlayer_State_Skill::CPlayer_State_Skill(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
	m_vecSkillState.push_back(nullptr);
	m_vecSkillState.push_back(new CPlayer_Skill_GolemFist(m_pOwner));
	m_vecSkillState.push_back(new CPlayer_Skill_Turtle(m_pOwner));
	m_vecSkillState.push_back(new CPlayer_Skill_Drill(m_pOwner));
	m_vecSkillState.push_back(new CPlayer_Skill_Lightning(m_pOwner));
	m_vecSkillState.push_back(new CPlayer_Skill_Missile(m_pOwner));

	m_eSkill = PLAYER_SKILL::NONE;
}

CPlayer_State_Skill::~CPlayer_State_Skill()
{
}

HRESULT CPlayer_State_Skill::Ready_State(void)
{
	
	if (m_vecSkillState[(_uint)m_eSkill])
	{
		m_vecSkillState[(_uint)m_eSkill]->Ready_State();
	}
	
	return S_OK;
}

_int CPlayer_State_Skill::Update_State(const _float& fTimeDelta)
{
	if (m_vecSkillState[(_uint)m_eSkill])
	{
		m_vecSkillState[(_uint)m_eSkill]->Update_State(fTimeDelta);
	}
	return 0;
}

void CPlayer_State_Skill::LateUpdate_State(void)
{
	if (m_vecSkillState[(_uint)m_eSkill])
	{
		m_vecSkillState[(_uint)m_eSkill]->LateUpdate_State();
	}
}

void CPlayer_State_Skill::Render_State(void)
{
	if (m_vecSkillState[(_uint)m_eSkill])
	{
		m_vecSkillState[(_uint)m_eSkill]->Render_State();
	}
}

void CPlayer_State_Skill::Reset_State(void)
{
}
