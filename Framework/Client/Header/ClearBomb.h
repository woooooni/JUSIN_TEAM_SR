#pragma once
#include "FieldObject.h"
class CClearBomb :   public CFieldObject
{
public:
	CLONE(CClearBomb)

protected:

	CClearBomb(LPDIRECT3DDEVICE9 p_Dev);
	CClearBomb(const CClearBomb& rhs);
	virtual ~CClearBomb();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CClearBomb* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

};

