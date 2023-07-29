#pragma once
#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END
class CCoin :
    public CItem
{
	CLONE(CCoin)

protected:
	explicit CCoin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCoin(const CCoin& rhs);
	virtual ~CCoin();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;



protected:
	HRESULT	Add_Component(void);

	_uint		m_iMoney;
	_bool		m_bIsCollided = false;
	_float		m_fColidedTime;

public:
	static	CCoin* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& pMoney, const _vec3& pPos);

protected:
	virtual void Free() override;
};

