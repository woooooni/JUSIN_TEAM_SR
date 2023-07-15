#pragma once
#include "Item.h"
class CEtcItem :    public CItem
{

protected:
	explicit CEtcItem(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	explicit CEtcItem(const CItem& rhs);
	virtual ~CEtcItem();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	virtual	CGameObject* Get_ByPool() override;
	virtual void				Add_Pool()override;

	virtual void		Free() override;
	CLONE(CEtcItem)

	HRESULT		Change_Item(const ITEM_CODE& pCode);

	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;


public:
	static		CEtcItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID, const ITEM_CODE& pCode = ITEM_CODE::LEAF);


protected:
	virtual HRESULT	Add_Component(void);

};

