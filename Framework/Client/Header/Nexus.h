#pragma once
#include "GameObject.h"
class CNexus : public CGameObject
{
	CLONE(CNexus)
private:
	explicit CNexus(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNexus(const CNexus& rhs);
	virtual ~CNexus();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

private:
	HRESULT			Ready_Component(void);


public:
	static CNexus* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

private:
	virtual void Free() override;
};

