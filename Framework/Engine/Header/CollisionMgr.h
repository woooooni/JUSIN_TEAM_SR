#pragma once
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Collider.h"
#include "GameObject.h"


union COLLIDER_ID
{
	struct
	{
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

BEGIN(Engine)
class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

public:
	HRESULT Ready_CollisionMgr(LPDIRECT3DDEVICE9 _pDevice);
	void Update_Collision();

public:
	void CheckGroupType(COLLISION_GROUP _eLeft, COLLISION_GROUP _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)COLLISION_GROUP::COLLIDE_END); }

public:
	void Add_CollisionGroup(CCollider* pCol, COLLISION_GROUP pState);



private:
	void CollisionUpdate(COLLISION_GROUP _eLeft, COLLISION_GROUP _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);


private:
	vector<CCollider*> m_vecCol[(_uint)COLLISION_GROUP::COLLIDE_END];

	map<ULONGLONG, _bool> m_mapColInfo;
	UINT m_arrCheck[(UINT)COLLISION_GROUP::COLLIDE_END];

	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
	virtual void	Free(void);

};

END
#endif

