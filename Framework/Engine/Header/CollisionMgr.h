#pragma once
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

private:
	/*bool IsCollision();*/

public:
	virtual void	Free(void);
};

END
#endif

