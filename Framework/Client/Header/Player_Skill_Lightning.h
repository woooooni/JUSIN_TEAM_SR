#pragma once
#include "Player_State.h"
class CPlayer_Skill_Lightning : public CPlayer_State
{
public:
	CPlayer_Skill_Lightning(CGameObject* _pOwner);
	~CPlayer_Skill_Lightning();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;



private:
	void					Key_Input(const _float& fTimeDelta);
	HRESULT					Shoot(void);

	

	_matrix					m_matRange;
	_vec3					m_vPos;
	_vec3					m_vDir;

	_float					m_fAimMoveSpeed;

	bool m_bSkillStart = false;
};

