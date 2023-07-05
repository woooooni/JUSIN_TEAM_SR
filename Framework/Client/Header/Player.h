#pragma once

#include "GameObject.h"
#include "Player_State.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CPlayer : public Engine::CGameObject
{
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);

public:
	void			Change_State(PLAYER_STATE _eState)
	{
		m_bStateChange = true;
		m_eChangeState = _eState;
	}
	void			Player_Move(_float fTimeDelta);

public:
	bool			Is_GetItem() { return m_bGetItem; }
	void			Set_GetItem(bool _bGetItem) { m_bGetItem = _bGetItem; }

	void			Set_BalloonFly(bool _bBalloonFly) { m_bBalloonFly = _bBalloonFly; }
	void			Set_Flying(bool _bFlying) { m_bFlying = _bFlying; }
	bool			Is_Flying() { return m_bFlying; }
	bool			Is_BalloonFly() { return m_bBalloonFly; }


private:
	_vec3			m_vDir;
	_float			m_fSpeed;

	_vec3			m_vDest;

	bool			m_bStateChange;
	PLAYER_STATE	m_eState;
	PLAYER_STATE	m_eChangeState;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;





	//플레이어 상태
	vector<CPlayer_State*> m_vecState;

	bool m_bGetItem = false;


	bool m_bBalloonFly = false;
	bool m_bFlying = false;
	//
};

