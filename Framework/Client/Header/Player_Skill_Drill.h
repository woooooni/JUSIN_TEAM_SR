#pragma once
#include "Player_State.h"
class CPlayer_Skill_Drill : public CPlayer_State
{
public:
	CPlayer_Skill_Drill(CGameObject* _pOwner);
	~CPlayer_Skill_Drill();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;



private:
	void					Update_Start(const _float& fTimeDelta);
	void					Update_InGround(const _float& fTimeDelta);
	void					Update_OutGround(const _float& fTimeDelta);

	void					LateUpdate_Start();
	void					LateUpdate_InGround();
	void					LateUpdate_OutGround();

	void					Key_Input(const _float& fTimeDelta);


	bool m_bSkillStart = false;

	DRILL_STATE m_eState;

	_float m_fSpeed;

	bool m_bJump = false;
	_float m_fMinHeight;
};

