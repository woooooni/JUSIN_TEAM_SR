#pragma once
#include "Player_State.h"
#include "GameObject.h"
#include "Effect_EyeTrail.h"

class CPlayer_State_Idle : public CPlayer_State
{
public:
	CPlayer_State_Idle(CGameObject* _pOwner);
	~CPlayer_State_Idle();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;



private:
	void					Update_Hat();

	void					Key_Input(const _float& fTimeDelta);

	OBJ_DIR	m_eDir;

	CEffect_EyeTrail* m_pEyeTrail = nullptr;
};

