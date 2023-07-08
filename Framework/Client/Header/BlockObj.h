#pragma once
#include "FieldObject.h"

class CBlockObj : public CFieldObject
{
	CLONE(CBlockObj)

protected:
	CBlockObj(LPDIRECT3DDEVICE9 p_Dev);
	CBlockObj(const CBlockObj& rhs);
	virtual ~CBlockObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CBlockObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos = { 0, 0, 0 }, _bool p_isFirstBlock = false);

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual HRESULT		Ready_Component();
	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	virtual void		Set_SubscribeEvent(_uint pEvent) override;


protected:
	_uint	m_iEventNum = 0;
	_uint	m_iFollowingEvent = 0;
	bool    m_bIsBlocking = false;


	void		Change_State();



};

