#pragma once
#include "FieldObject.h"
class CRabitObj :
    public CFieldObject
{
public:
	CLONE(CRabitObj);

protected:
	CRabitObj(LPDIRECT3DDEVICE9 p_Dev);
	CRabitObj(const CRabitObj& rhs);
	virtual ~CRabitObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CRabitObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos = { 0, 0, 0 });

	void			Set_Up();

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;


protected:
	_vec3		m_vRabitPos;
	_float		m_fMaxHeight;
	_bool		m_bisUp;
	_float		m_fUpTime;
	_bool		m_bHitted;
	_float		m_fHitTIme;

};

