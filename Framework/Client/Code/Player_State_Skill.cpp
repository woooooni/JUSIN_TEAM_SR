#include "../Header/Player_State_Skill.h"
#include "Engine_Enum.h"
#include "Player_State.h"
#include "Player_Skill_GolemFist.h"

CPlayer_State_Skill::CPlayer_State_Skill(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
	for (_uint i = 0; (_uint)PLAYER_SKILL::SKILLEND > i; ++i)
		m_vecSkillState.push_back(nullptr);

	m_eSkill = PLAYER_SKILL::SKILLEND;
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

void CPlayer_State_Skill::Add_Skill(PLAYER_SKILL Player_Skill)
{
	switch (Player_Skill)
	{
	case PLAYER_SKILL::GOLEMFIST:
		if(!m_vecSkillState[(_uint)PLAYER_SKILL::GOLEMFIST])
			m_vecSkillState[(_uint)PLAYER_SKILL::GOLEMFIST] = new CPlayer_Skill_GolemFist(m_pOwner);
		break;
	}
}
