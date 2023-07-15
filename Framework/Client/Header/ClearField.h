#pragma once
#include "FieldObject.h"
class CClearField :    public CFieldObject
{

protected:

	CClearField(LPDIRECT3DDEVICE9 p_Dev);
	CClearField(const CClearField& rhs);
	virtual ~CClearField();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	CLONE(CClearField)

	virtual void    Free() override;

	static			CClearField* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;


protected:
	_bool		m_bCreating;
	_float		m_fFrame;
	_float		m_fMaxFrame;
};

