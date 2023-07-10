#pragma once
#include "FieldObject.h"
class CGrass : public CFieldObject
{
public:
	CLONE(CGrass)

protected:
	CGrass(LPDIRECT3DDEVICE9 p_Dev);
	CGrass(const CGrass& rhs);
	virtual ~CGrass();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CGrass* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

	vector<_float>		m_vecGrassSize;
	vector<_vec3>		m_vecGrassPos;

};

