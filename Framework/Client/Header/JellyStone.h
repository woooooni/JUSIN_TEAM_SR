#pragma once
#include "FieldObject.h"
class CJellyStone :	public CFieldObject
{
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

	static			CJellyStone* Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLLOR_NORMAL p_Color,const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)override;
	virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID) override;

	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;


protected:
	JELLY_COLLOR_NORMAL			m_eColor;
	_bool						m_bCreatedCombine = false;

};

