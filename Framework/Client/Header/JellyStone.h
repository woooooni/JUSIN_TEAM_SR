#pragma once
#include "FieldObject.h"
class CJellyStone : public CFieldObject
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

	static			CJellyStone* Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLOR p_Color = JELLY_COLOR::JELLY_END, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;
	virtual void Reset_Event()override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;

	const JELLY_COLOR& Get_JellyColor() { return m_eColor; }
	void		Set_Created_False() { m_bCreatedCombine = false; }

	void		Set_Color(const JELLY_COLOR& pColor);

	virtual		CGameObject* Get_GrabObj()override;


	const  _bool& Get_Created() { return m_bCreatedCombine; }

protected:
	JELLY_COLOR			m_eColor;
	JELLY_COLOR			m_eOriginColor;
	_vec3				m_vOriginPos;
	_bool						m_bCreatedCombine = false;
	list<CGameObject*>	m_listCreatedJellys;

	bool Check_Child(const JELLY_COLOR& pColor);
};

