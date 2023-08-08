#pragma once
#include "GameObject.h"

class CDoorEnter final : public CGameObject
{
	CLONE(CDoorEnter)

protected:
	explicit CDoorEnter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoorEnter(const CDoorEnter& rhs);
	virtual ~CDoorEnter();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

private:
	virtual HRESULT	Ready_Component(void);

private:

public:
	static	CDoorEnter* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

public:


private:
	bool m_bStart = false;

};

