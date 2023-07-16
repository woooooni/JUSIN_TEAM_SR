#pragma once
#include       "FieldObject.h"
class CHoleObj :    public CFieldObject
{

protected:
	CHoleObj(LPDIRECT3DDEVICE9 p_Dev);
	CHoleObj(const CHoleObj& rhs);
	virtual ~CHoleObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CHoleObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos = { 0, 0, 0 });

	CLONE(CHoleObj);

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

protected:
	_bool	m_bIn;

};

