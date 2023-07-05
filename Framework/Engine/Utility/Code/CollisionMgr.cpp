#include "..\..\Header\CollisionMgr.h"
#include	"BoxCollider.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr(void)
{
	for (auto& iter : m_vecCol)
	{
		iter.reserve(sizeof(CCollider*) * 100);
	}
}

Engine::CCollisionMgr::~CCollisionMgr(void)
{
	Free();
}

void CCollisionMgr::Free(void)
{
	
}

void CCollisionMgr::Add_CollisionGroup(CCollider* pCol, COLLISION_GROUP pState)
{
	m_vecCol[(_uint)pState].push_back(pCol);
}

void CCollisionMgr::Group_Collide(COLLISION_GROUP pStateA, COLLISION_GROUP pStateB)
{
	COLLISION_DIR dir = COLLISION_DIR::NOT_COLLIDE;

	switch (pStateA)
	{
	case Engine::COLLIDE_STATE::COLLIDE_WALL:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_PLAYER:
		for (auto& iter : (m_vecCol[(_uint)pStateB]))
		{
			switch (pStateB)
			{
			case Engine::COLLIDE_STATE::COLLIDE_WALL:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_PLAYER:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_MOBBODY:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_GRAB:
				if ((_uint)(dir = Check_Collision((m_vecCol[(_uint)pStateA])[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB], iter)))
				{

				}
				break;
			case Engine::COLLIDE_STATE::COLLIDE_PUSH:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_BREAK:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_BULLET:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_BOMB:
				break;
			case Engine::COLLIDE_STATE::COLLIDE_END:
				break;
			default:
				break;
			}
		}
		break;
	case Engine::COLLIDE_STATE::COLLIDE_MOBBODY:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_GRAB:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_PUSH:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_BREAK:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_BULLET:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_BOMB:
		break;
	case Engine::COLLIDE_STATE::COLLIDE_END:
		break;
	default:
		break;
	}
}



COLLISION_DIR CCollisionMgr::Check_Collision( CGameObject* objA,  CGameObject* objB)
{
	CBoxCollider* colA = dynamic_cast<CBoxCollider*>(objA->Get_ColliderCom());
	CBoxCollider* colB = dynamic_cast<CBoxCollider*>(objB->Get_ColliderCom());

	NULL_CHECK_RETURN(colA, COLLISION_DIR::NOT_COLLIDE);
	NULL_CHECK_RETURN(colB, COLLISION_DIR::NOT_COLLIDE);


	const _vec3& scaleA = colA->Get_Scale();
	const _vec3& posA = colA->Get_Pos();
	const _vec3& scaleB = colB->Get_Scale();
	const _vec3& posB = colB->Get_Pos();

	if (fabs(posB.x - posA.x) < 0.5f * (scaleA.x + scaleB.x)
		&& fabs(posB.y - posA.y) < 0.5f * (scaleA.y + scaleB.y)
		&& fabs(posB.z - posA.z) < 0.5f * (scaleA.z + scaleB.z))
	{
		float colX = 0.5f * (scaleA.x + scaleB.x) - fabs(posB.x - posA.x);
		float colY = 0.5f * (scaleA.y + scaleB.y) - fabs(posB.y - posA.y);
		float colZ = 0.5f * (scaleA.z + scaleB.z) - fabs(posB.z - posA.z);

		if (colX < colY && colX < colZ)
		{
			if (posB.x > posA.x)
			{
				return COLLISION_DIR::DIR_L;
			}
			else
			{
				return COLLISION_DIR::DIR_R;
			}
		}
		else if (colY < colX && colY < colZ)
		{
			if (posB.y > posA.y)
			{
				return COLLISION_DIR::DIR_D;
			}
			else
			{
				return COLLISION_DIR::DIR_U;
			}
		}
		else if (colZ < colX && colZ < colY)
		{
			if (posB.z > posA.z)
			{
				return COLLISION_DIR::DIR_FAR;
			}
			else
			{
				return COLLISION_DIR::DIR_NEAR;
			}
		}

	}



	

	return COLLISION_DIR::NOT_COLLIDE;
}

COLLISION_DIR CCollisionMgr::Check_Collision(CCollider* objA, CCollider* objB)
{
	CBoxCollider* colA = dynamic_cast<CBoxCollider*>(objA);
	CBoxCollider* colB = dynamic_cast<CBoxCollider*>(objB);

	NULL_CHECK_RETURN(colA, COLLISION_DIR::NOT_COLLIDE);
	NULL_CHECK_RETURN(colB, COLLISION_DIR::NOT_COLLIDE);

	const _vec3& scaleA = colA->Get_Scale();
	const _vec3& posA = colA->Get_Pos();
	const _vec3& scaleB = colB->Get_Scale();
	const _vec3& posB = colB->Get_Pos();

	if (fabs(posB.x - posA.x) < 0.5f * (scaleA.x + scaleB.x)
		&& fabs(posB.y - posA.y) < 0.5f * (scaleA.y + scaleB.y)
		&& fabs(posB.z - posA.z) < 0.5f * (scaleA.z + scaleB.z))
	{
		float colX = 0.5f * (scaleA.x + scaleB.x) - fabs(posB.x - posA.x);
		float colY = 0.5f * (scaleA.y + scaleB.y) - fabs(posB.y - posA.y);
		float colZ = 0.5f * (scaleA.z + scaleB.z) - fabs(posB.z - posA.z);

		if (colX < colY && colX < colZ)
		{
			if (posB.x > posA.x)
			{
				return COLLISION_DIR::DIR_L;
			}
			else
			{
				return COLLISION_DIR::DIR_R;
			}
		}
		else if (colY < colX && colY < colZ)
		{
			if (posB.y > posA.y)
			{
				return COLLISION_DIR::DIR_D;
			}
			else
			{
				return COLLISION_DIR::DIR_U;
			}
		}
		else if (colZ < colX && colZ < colY)
		{
			if (posB.z > posA.z)
			{
				return COLLISION_DIR::DIR_FAR;
			}
			else
			{
				return COLLISION_DIR::DIR_NEAR;
			}
		}

	}
	return COLLISION_DIR::NOT_COLLIDE;

}
