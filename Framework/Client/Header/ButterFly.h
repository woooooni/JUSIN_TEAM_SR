#pragma once
#include "Item.h"
class CButterFly :   public CItem
{
public:
	CLONE(CButterFly)

protected:
	CButterFly(LPDIRECT3DDEVICE9 p_Dev);
	CButterFly(const CButterFly& rhs);
	virtual ~CButterFly();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CButterFly* Create(LPDIRECT3DDEVICE9 p_Dev, ITEM_CODE p_Code, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

	virtual	CGameObject* Get_ByPool() override;
	virtual void				Add_Pool() override;

	HRESULT		Change_Item(const ITEM_CODE& pCode);


protected:
	HRESULT			Ready_Component();
	_vec3	m_vMovingDir;
	_float	m_fChangeTime;

};

