#pragma once
#include "Bullet.h"
class CTurretBullet :  public CBullet
{
protected:
	explicit CTurretBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	explicit CTurretBullet(const CTurretBullet& rhs);
	virtual ~CTurretBullet();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;



	CLONE(CTurretBullet);

public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;


public:
	static CTurretBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pPos = { 0, 0, 0 }, const _vec3& pDir = {1, 0, 0});

	void			Set_Start(const _vec3& pPos, const _vec3& pDir);

protected:
	virtual void Free() override;
	_vec3		m_vDirection;
	HRESULT		Ready_Component();

};

