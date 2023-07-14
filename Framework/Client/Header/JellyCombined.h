#pragma once
#include "FieldObject.h"
class CJellyCombined :    public CFieldObject
{
	CLONE(CJellyCombined)

protected:
	CJellyCombined(LPDIRECT3DDEVICE9 p_Dev);
	CJellyCombined(const CJellyCombined& rhs);
	virtual ~CJellyCombined();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CJellyCombined* Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLLOR_COMBINE p_Color = JELLY_COLLOR_COMBINE::JELLY_COMBINEEND, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

	virtual	CGameObject* Get_GrabObj() override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

	const JELLY_COLLOR_COMBINE& Get_JellyColor() { return m_eColor; }
	void	Set_JellyColor(const JELLY_COLLOR_COMBINE& pColor);

protected:
	JELLY_COLLOR_COMBINE	m_eColor;
	bool		Check_Child(const JELLY_COLLOR_NORMAL& pColor);

};

