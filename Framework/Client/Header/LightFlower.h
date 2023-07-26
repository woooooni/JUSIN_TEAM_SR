#pragma once
#include "FieldObject.h"

class CBalpanObj;

class CLightFlower :    public CFieldObject
{
public:
	CLONE(CLightFlower)

protected:
	CLightFlower(LPDIRECT3DDEVICE9 p_Dev);
	CLightFlower(const CLightFlower& rhs);
	virtual ~CLightFlower();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CLightFlower* Create(LPDIRECT3DDEVICE9 p_Dev, CGameObject* p_Balpan, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

	_matrix		AreaWorld;
	_bool		m_bIsOpened;
	JELLY_COLOR		m_eColor;
	CBalpanObj* m_pBalPan;

};

