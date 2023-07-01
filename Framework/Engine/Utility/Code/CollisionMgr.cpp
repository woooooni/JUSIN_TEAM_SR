#include "..\..\Header\CollisionMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr(void)
{

}

Engine::CCollisionMgr::~CCollisionMgr(void)
{
	Free();
}

void CCollisionMgr::Free(void)
{
}
