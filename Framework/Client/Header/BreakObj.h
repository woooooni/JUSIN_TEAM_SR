#pragma once
#include "FieldObject.h"
class CBreakObj :    public CFieldObject
{
public:
	CLONE(CBreakObj);

protected:
	CBreakObj(LPDIRECT3DDEVICE9 p_Dev);
	CBreakObj(const CBreakObj& rhs);
	virtual ~CBreakObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CBreakObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	
protected:
	_bool	m_bIsBreaking;

};

