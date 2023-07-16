#pragma once
#include "Player_State.h"
class CPlayer_State_GetItem : public CPlayer_State
{
public:
	CPlayer_State_GetItem(CGameObject* _pOwner);
	~CPlayer_State_GetItem();

public:

	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;
	virtual void			Reset_State(void) override;

private:
	bool	m_bFinished = false;
	bool	m_bEffect = false;


	_vec3	m_vStartPos;


	void					Update_Hat();
	void					Set_Hat();


	vector<_vec3> m_vecHatPos;
	float m_fAngle[6];
	float m_fScale[6];

	_uint m_iIndex;
	bool m_bReverse = false;

	CGameObject* m_pEffect = nullptr;
};

