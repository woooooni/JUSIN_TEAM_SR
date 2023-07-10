#pragma once
#include "Player_State.h"
class CPlayer_State_Lift : public CPlayer_State
{
public:
	CPlayer_State_Lift(CGameObject* _pOwner);
	~CPlayer_State_Lift();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;

private:
	_int			Update_LiftReady(const _float& fTimeDelta);
	_int			Update_LiftUp(const _float& fTimeDelta);
	_int			Update_LiftIdle(const _float& fTimeDelta);
	_int			Update_LiftDown(const _float& fTimeDelta);
	_int			Update_LiftWalk(const _float& fTimeDelta);



	LIFT_STATE m_eLiftState;


	_float m_fAccTime;
	_float m_fKeyDelayTime;

	_float m_fLiftTime;


	_vec3 m_vTargetDir;
	_vec3 m_vCurrDir;
	_vec3 m_vRot;

	void					Update_Hat_LiftUp();
	void					Set_Hat_LiftUp();

	void					Update_Hat_LiftIdle();
	void					Set_Hat_LiftIdle();

	void					Update_Hat_LiftWalk();
	void					Set_Hat_LiftWalk();

	void					Update_Hat_LiftDown();
	void					Set_Hat_LiftDown();

	vector<_vec3> m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_END][8];
	float m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_END][8];

	_vec3 m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_END];
	float m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_END];

	vector<_vec3> m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_END][10];
	float m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_END][10];

	vector<_vec3> m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_END][8];
	float m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_END][8];
};

