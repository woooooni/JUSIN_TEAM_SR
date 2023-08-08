#pragma once
#include "Player_State.h"
class CPlayer_State_Push : public CPlayer_State
{
public:
	CPlayer_State_Push(CGameObject* _pOwner);
	~CPlayer_State_Push();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;


private:
	void					Key_Input(const _float& fTimeDelta);

	OBJ_DIR					m_eStartDir;

	_float m_fAccTime;
	_float m_fKeyDelayTime;

	void					Update_Hat();
	void					Set_Hat();


	vector<_vec3> m_vecHatPos[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle[(_uint)OBJ_DIR::DIR_END][10];
	float m_fScale[(_uint)OBJ_DIR::DIR_END][10];
};

