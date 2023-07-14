#pragma once
#include "FieldObject.h"
class CJellyStone :	public CFieldObject
{
	CLONE(CJellyStone)
protected:
	CJellyStone(LPDIRECT3DDEVICE9 p_Dev);
	CJellyStone(const CJellyStone& rhs);
	virtual ~CJellyStone();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CJellyStone* Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLLOR_NORMAL p_Color = JELLY_COLLOR_NORMAL::JELLY_NORMALEND,const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

	const JELLY_COLLOR_NORMAL& Get_JellyColor() { return m_eColor; }
	void		Set_Created_False() { m_bCreatedCombine = false; }

	void		Set_Color(const JELLY_COLLOR_NORMAL& pColor);

protected:
	JELLY_COLLOR_NORMAL			m_eColor;
	_bool						m_bCreatedCombine = false;

};

