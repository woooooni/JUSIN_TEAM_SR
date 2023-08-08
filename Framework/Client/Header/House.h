#pragma once
#include "FixedObj.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CHouse : public CFixedObj
{
	CLONE(CHouse)

protected:
	explicit CHouse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHouse(const CHouse& rhs);
	virtual ~CHouse();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;



public:
	// �浹 ȣ��
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;

private:
	HRESULT Ready_Component();

public:
	static CHouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual void Free() override;
};

