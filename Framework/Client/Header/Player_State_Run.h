#pragma once
#include "Player_State.h"
#include "Effect_Trail.h"

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

	void					Change_RunDir();
	void					Update_EffectOffset();
	void					Set_EffectOffset();

	_float m_fAccTime;
	_float m_fKeyDelayTime;

	vector<_vec3> m_vecHatPos[(_uint)OBJ_DIR::DIR_END];
	float m_fAngle[(_uint)OBJ_DIR::DIR_END][10];
	float m_fScale[(_uint)OBJ_DIR::DIR_END][10];

	CEffect_Trail* m_pLeftEffect = nullptr;
	CEffect_Trail* m_pRightEffect = nullptr;


	_uint m_iCurrIndex;
	deque<_uint> m_deqIndex;

	OBJ_DIR m_eRunDir;


	_vec3 m_vecLeftOffset[(_uint)OBJ_DIR::DIR_END][8];
	_vec3 m_vecRightOffset[(_uint)OBJ_DIR::DIR_END][8];
};


