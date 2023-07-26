#pragma once
#include "GameObject.h"

class CDrawingEnter final : public CGameObject
{
	CLONE(CDrawingEnter)

protected:
	explicit CDrawingEnter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDrawingEnter(const CDrawingEnter& rhs);
	virtual ~CDrawingEnter();

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
	static	CDrawingEnter* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;

public:


private:
	bool m_bStart = false;
	bool m_bFinish = false;

	_uint m_iAlpha;
};

