#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CPlayer_Bullet_GolemFist : public CBullet
{
	CLONE(CPlayer_Bullet_GolemFist)
private:
	explicit CPlayer_Bullet_GolemFist(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Bullet_GolemFist(const CPlayer_Bullet_GolemFist& rhs);
	virtual ~CPlayer_Bullet_GolemFist();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);
public:


public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;



private:

public:
	static CPlayer_Bullet_GolemFist* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	_float m_fStopTime;
	_float m_fAccTime;

	bool m_bEffect = false;
private:
	virtual void Free() override;
};

