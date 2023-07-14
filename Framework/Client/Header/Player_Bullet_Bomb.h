#pragma once
#include "Bullet.h"
class CPlayer_Bullet_Bomb : public CBullet
{
	CLONE(CPlayer_Bullet_Bomb)
private:
	explicit CPlayer_Bullet_Bomb(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pOwner);
	explicit CPlayer_Bullet_Bomb(const CPlayer_Bullet_Bomb& rhs);
	virtual ~CPlayer_Bullet_Bomb();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);
public:

	void			Shoot(CGameObject* _pTarget, _vec3& _vDir, _float _fPower, _vec3& _vPos);
public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;



private:

public:
	static CPlayer_Bullet_Bomb* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pOwner);

private:
	 CGameObject* m_pTarget;
	 _vec3 m_vTargetPos;

	 _float m_fMovePower;

	 _vec3 m_vScale;
	 _float m_fOriginMaxVel;
	 _float m_fMaxVel;
private:
	virtual void Free() override;
};

