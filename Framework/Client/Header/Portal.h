#pragma once
#include "GameObject.h"


class CPortal final : public CGameObject
{
	CLONE(CPortal)

protected:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene);
	explicit CPortal(const CPortal& rhs);
	virtual ~CPortal();

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
	SCENE_TYPE m_eNextScene;

public:
	static	CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene);

protected:
	virtual void Free() override;
};