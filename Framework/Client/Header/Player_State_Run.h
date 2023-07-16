#pragma once
#include "Player_State.h"
class CPlayer_State_Run : public CPlayer_State
{
public:
	CPlayer_State_Run(CGameObject* _pOwner);
	~CPlayer_State_Run();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;

private:
	void					Key_Input(const _float& fTimeDelta);
	void					Update_Hat();
	void					Set_Hat();

	_float m_fAccTime;
	_float m_fKeyDelayTime;

	vector<_vec3> m_vecHatPos[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle[(_uint)OBJ_DIR::DIR_END][10];
	float m_fScale[(_uint)OBJ_DIR::DIR_END][10];
};


