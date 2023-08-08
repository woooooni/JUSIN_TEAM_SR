#pragma once
#include "Player_State.h"
class Player_State_BallonFly : public CPlayer_State
{
public:
	Player_State_BallonFly(CGameObject* _pOwner);
	~Player_State_BallonFly();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;

private:
	_int			Update_FlyReady(const _float& fTimeDelta);
	_int			Update_Flying(const _float& fTimeDelta);
	_int			Update_FlyEnd(const _float& fTimeDelta);


	bool	m_bFinished = false;
	BALLOONFLY_STATE m_eFlyState;


};

