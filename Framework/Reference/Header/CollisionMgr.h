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

	void		Add_CollisionGroup(CCollider*		pCol, COLLISION_GROUP pState);

	void		Group_Collide(COLLISION_GROUP pStateA, COLLISION_GROUP pStateB);


	COLLISION_DIR			Check_Collision(CGameObject* objA,  CGameObject* objB);
	COLLISION_DIR			Check_Collision(CCollider* objA, CCollider* objB);


private:
	vector<CCollider*> m_vecCol[(_uint)COLLISION_GROUP::COLLIDE_END];

};

END
#endif

