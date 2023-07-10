#pragma once
#include "Player_State.h"
class CPlayer_State_Jump : public CPlayer_State
{
public:
	CPlayer_State_Jump(CGameObject* _pOwner);
	~CPlayer_State_Jump();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;

private:
	_vec3 m_vDir;

	bool m_bJump = false;

	void					Update_Hat();
	void					Set_Hat();


	vector<_vec3> m_vecHatPos[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle[(_uint)OBJ_DIR::DIR_END][12];
	float m_fScale[(_uint)OBJ_DIR::DIR_END][12];
};

