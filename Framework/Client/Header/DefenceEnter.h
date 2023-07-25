#pragma once
#include "GameObject.h"

class CDefenceEnter final : public CGameObject
{
	CLONE(CDefenceEnter)

protected:
	explicit CDefenceEnter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefenceEnter(const CDefenceEnter& rhs);
	virtual ~CDefenceEnter();

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
	static	CDefenceEnter* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

public:


private:
	bool m_bStart = false;

	_float m_fCameraTime;
	_float m_fAccTime;
};

