#pragma once
#include "Player_State.h"
class CPlayer_Skill_GolemFist : public CPlayer_State
{
public:
	CPlayer_Skill_GolemFist(CGameObject* _pOwner);
	~CPlayer_Skill_GolemFist();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;



private:
	void					Key_Input(const _float& fTimeDelta);
	void					Shoot(void);

	_matrix					m_matRange;
	_vec3					m_vPlusScale;
	_vec3					m_vScale;
	_vec3					m_vPos;
	_vec3					m_vDir;

	_float					m_fRotAngle;
	_float					m_fAimMoveSpeed;

	CGameObject*			m_pGolemFist;

	bool m_bSkillStart = false;
};

