#pragma once
#include "FieldObject.h"
class CBarrelBomb :
    public CFieldObject
{
	CLONE(CBarrelBomb);

protected:
	CBarrelBomb(LPDIRECT3DDEVICE9 p_Dev);
	CBarrelBomb(const CBarrelBomb& rhs);
	virtual ~CBarrelBomb();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CBarrelBomb* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

	void	Reset();


protected:
	_bool		m_bHitted;
	_uint		m_iExplodeEvent;
	_float		m_fExplodeTime;
	_bool		m_bExplosing;
	_bool		m_bIsPlusAlpha;
	_float		m_fBlurAlpha;

};

