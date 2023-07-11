#pragma once
#include "Player_State.h"
class CPlayer_State_Skill : public CPlayer_State
{
public:
	CPlayer_State_Skill(CGameObject* _pOwner);
	~CPlayer_State_Skill();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;

	

	bool					Is_HaveSkill() 
	{
		if (m_eSkill == PLAYER_SKILL::SKILLEND || !m_vecSkillState[(_uint)m_eSkill])
		{
			return false;
		}
		return true;
	}

	PLAYER_SKILL			Get_Skill() { return m_eSkill; }
	void					Set_Skill(PLAYER_SKILL Player_Skill) { m_eSkill = Player_Skill; }

private:
	vector<CPlayer_State*>	m_vecSkillState;
	PLAYER_SKILL			m_eSkill;
};

