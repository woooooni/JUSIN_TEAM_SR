#pragma once
#include "FieldObject.h"
class CBreakStone :    public CFieldObject
{

protected:
	CBreakStone(LPDIRECT3DDEVICE9 p_Dev);
	CBreakStone(const CBreakStone& rhs);
	virtual ~CBreakStone();
public:
	CLONE(CBreakStone)



public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CBreakStone* Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLOR pColor ,const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;



protected:
	JELLY_COLOR		m_eColor;
	_bool			m_bBreak;

};

