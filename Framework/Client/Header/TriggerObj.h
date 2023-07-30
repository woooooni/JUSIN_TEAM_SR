#pragma once
#include	"GameObject.h"
class CTriggerObj : public CGameObject
{
public:

	enum class COLLIDE_EVENT_TYPE
	{
		ENTER,
		STAY,
		EXIT,
		TYPE_END
	};
	CLONE(CTriggerObj);

protected:
	CTriggerObj(LPDIRECT3DDEVICE9 p_Dev);
	CTriggerObj(const CTriggerObj& rhs);
	virtual ~CTriggerObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum) override;
	virtual void Reset_Event() override;


	virtual void    Free() override;

	static			CTriggerObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos = { 0, 0, 0 });

	void	Add_Trigger(void(*pFunc)(), const COLLIDE_EVENT_TYPE& pType)
	{
		m_pCollideEvent[(_uint)pType] = pFunc;
	}

	void		Set_Scale(const _vec3& pVec);
	void		Set_Once() { m_bIsOnce = true; }
	void		Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

	void		Set_EventTrigger(const _uint& pEventNum, void(*pFunc)(), const _bool& pPushBack = true);


public:
	virtual void Collision_Enter(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

protected:
	void (*m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::TYPE_END])();
	list<void(*)()>m_pEventEvent;

	_bool		m_bIsOnce;
	CGameObject* m_pTarget;
	list<_uint>		m_iSubscribedEventNum;

};

