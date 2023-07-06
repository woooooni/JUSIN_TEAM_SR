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
		if (!m_bStateChange)
		{
			m_bStateChange = true;
			m_eChangeState = _eState;
		}
	}
	void			Player_Move(_float fTimeDelta);

public:
	// 충돌 호출
	virtual void Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID) override;

	CCollider* Get_PlayerCol(COLLIDER_PLAYER _eCollider) { return m_pCollider[(_uint)_eCollider]; }

	bool			Is_GetItem() { return m_bGetItem; }
	void			Set_GetItem(bool _bGetItem) { m_bGetItem = _bGetItem; }

	void			Set_BalloonFly(bool _bBalloonFly) { m_bBalloonFly = _bBalloonFly; }
	void			Set_Flying(bool _bFlying) { m_bFlying = _bFlying; }
	bool			Is_Flying() { return m_bFlying; }
	bool			Is_BalloonFly() { return m_bBalloonFly; }

	bool			Is_Push() { return m_bPush; }
	void			Set_Push(bool _bPush) { m_bPush = _bPush; }

	void			Set_Grab(bool _bGrab) { m_bGrab = _bGrab; }
	bool			Is_Grab() { return m_bGrab; }

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
	void Collision_Stay_Push(CGameObject* pCollisionObj, UINT _iColliderID);
	void Collision_Stay_Grab(CGameObject* pCollisionObj, UINT _iColliderID);

private:
	virtual void Free() override;

	CCollider* m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_END];



	//플레이어 상태
	vector<CPlayer_State*> m_vecState;

	bool m_bGetItem = false;


	bool m_bBalloonFly = false;
	bool m_bFlying = false;

	bool m_bPush = false;

	bool m_bGrab = false;
	//
};

