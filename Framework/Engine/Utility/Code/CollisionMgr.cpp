#include "..\..\Header\CollisionMgr.h"
#include	"BoxCollider.h"

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

void CCollisionMgr::Add_CollisionGroup(CCollider* pCol)
{
	m_vecCol.push_back(pCol);
}

void CCollisionMgr::Group_Collide()
{
	for (size_t i = 0; i < (m_vecCol.size() - 1); i++)
	{
		for (size_t j = i + 1; j < m_vecCol.size(); j++)
		{
			Check_Collision(m_vecCol[i], m_vecCol[j]);
		}
	}

	m_vecCol.clear();
}

HRESULT CCollisionMgr::Check_Collision( CGameObject* objA,  CGameObject* objB)
{
	CBoxCollider* colA = dynamic_cast<CBoxCollider*>(objA->Get_ColliderCom());
	CBoxCollider* colB = dynamic_cast<CBoxCollider*>(objB->Get_ColliderCom());

	NULL_CHECK_RETURN(colA, E_FAIL);
	NULL_CHECK_RETURN(colB, E_FAIL);


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
				MSG_BOX("哭率面倒");
			}
			else
			{
				MSG_BOX("坷弗率面倒");
			}
		}
		else if (colY < colX && colY < colZ)
		{
			if (posB.y > posA.y)
			{
				MSG_BOX("酒贰率面倒");
			}
			else
			{
				MSG_BOX("困率面倒");
			}
		}
		else if (colZ < colX && colZ < colY)
		{
			if (posB.z > posA.z)
			{
				MSG_BOX("菊率面倒");
			}
			else
			{
				MSG_BOX("第率面倒");
			}
		}

		return S_OK;
	}



	

	return E_FAIL;
}

HRESULT CCollisionMgr::Check_Collision(CCollider* objA, CCollider* objB)
{
	CBoxCollider* colA = dynamic_cast<CBoxCollider*>(objA);
	CBoxCollider* colB = dynamic_cast<CBoxCollider*>(objB);

	NULL_CHECK_RETURN(colA, E_FAIL);
	NULL_CHECK_RETURN(colB, E_FAIL);

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
				MSG_BOX("哭率面倒");
			}
			else
			{
				MSG_BOX("坷弗率面倒");
			}
		}
		else if (colY < colX && colY < colZ)
		{
			if (posB.y > posA.y)
			{
				MSG_BOX("酒贰率面倒");
			}
			else
			{
				MSG_BOX("困率面倒");
			}
		}
		else if (colZ < colX && colZ < colY)
		{
			if (posB.z > posA.z)
			{
				MSG_BOX("菊率面倒");
			}
			else
			{
				MSG_BOX("第率面倒");
			}
		}

		return S_OK;
	}





	return E_FAIL;
}
