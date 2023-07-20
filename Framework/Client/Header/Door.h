#pragma once
#include "FixedObj.h"

BEGIN(Engine)


class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;


END


class CDoor : public CFixedObj
{
	CLONE(CDoor)

protected:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoor(const CDoor& rhs);
	virtual ~CDoor();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;


public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum) override;

private:
	HRESULT Ready_Component();

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual void Free() override;
};

