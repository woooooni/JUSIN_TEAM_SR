#pragma once
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Collider.h"
#include "GameObject.h"


BEGIN(Engine)
class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

public:
	virtual void	Free(void);

	void		Add_CollisionGroup(CCollider*		pCol);

	void		Group_Collide();

	HRESULT			Check_Collision( CGameObject* objA,  CGameObject* objB);
	HRESULT			Check_Collision(CCollider* objA, CCollider* objB);


private:
	vector<CCollider*> m_vecCol;

};

END
#endif

