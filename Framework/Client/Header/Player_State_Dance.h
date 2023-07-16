#pragma once
#include "Player_State.h"
class CPlayer_State_Dance : public CPlayer_State
{
public:
	CPlayer_State_Dance(CGameObject* _pOwner);
	~CPlayer_State_Dance();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;

private:
	void					Key_Input(const _float& fTimeDelta);

	bool	m_bFinished = false;

	_float m_fAccTime;
	_float m_fDrawTime;


	void					Update_Hat();
	void					Set_Hat();


	vector<_vec3> m_vecHatPos;
	float m_fAngle[14];
	float m_fScale[14];
};

