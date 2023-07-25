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
	virtual _int		Update_Object(const _float & fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CTriggerObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos = { 0, 0, 0 });

	void	Add_Trigger(void(*pFunc)(), const COLLIDE_EVENT_TYPE& pType)
	{
		m_pCollideEvent[(_uint)pType] = pFunc;
	}

	void		Set_Scale(const _vec3& pVec);

public:
	virtual void Collision_Enter(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider * pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

protected:
	void (*m_pCollideEvent[(_uint)COLLIDE_EVENT_TYPE::TYPE_END])();

};

