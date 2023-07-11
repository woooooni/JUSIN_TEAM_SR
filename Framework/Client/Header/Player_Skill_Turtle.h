#pragma once
#include "Player_State.h"
class CPlayer_Skill_Turtle : public CPlayer_State
{
public:
	CPlayer_Skill_Turtle(CGameObject* _pOwner);
	~CPlayer_Skill_Turtle();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;



private:
	void					Key_Input(const _float& fTimeDelta);

	CGameObject* m_pBarrier;

	bool m_bSkillStart = false;
};

