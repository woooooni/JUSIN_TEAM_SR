#include "..\..\Header\CollisionMgr.h"
#include	"BoxCollider.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr(void)
{
	for (auto& iter : m_vecCol)
	{
		iter.reserve(100);
	}
	Reset();
}

Engine::CCollisionMgr::~CCollisionMgr(void)
{
	Free();
}

HRESULT CCollisionMgr::Ready_CollisionMgr(LPDIRECT3DDEVICE9 _pDevice)
{
	m_pGraphicDev = _pDevice;
	_pDevice->AddRef();

	CheckGroupType(COLLISION_GROUP::COLLIDE_PLAYER, COLLISION_GROUP::COLLIDE_MONSTER);

	return S_OK;
}

void CCollisionMgr::Update_Collision()
{
	for (UINT iRow = 0; iRow < (UINT)COLLISION_GROUP::COLLIDE_END; iRow++)
	{
		for (UINT iCol = iRow; iCol < (UINT)COLLISION_GROUP::COLLIDE_END; iCol++)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionUpdate((COLLISION_GROUP)iRow, (COLLISION_GROUP)iCol);
			}
		}
	}
}

void CCollisionMgr::Add_CollisionGroup(CCollider* pCol, COLLISION_GROUP _eGroup)
{
	m_vecCol[(_uint)_eGroup].push_back(pCol);
}

void CCollisionMgr::CheckGroupType(COLLISION_GROUP _eLeft, COLLISION_GROUP _eRight)
{
	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

void CCollisionMgr::CollisionUpdate(COLLISION_GROUP _eLeft, COLLISION_GROUP _eRight)
{
	map<ULONGLONG, _bool>::iterator iter;
	for (auto& lCollider : m_vecCol[(_uint)_eLeft])
	{
		if (!lCollider->Is_Active() || nullptr == lCollider)
			continue;

		for (auto& rCollider : m_vecCol[(_uint)_eRight])
		{
			if (!rCollider->Is_Active() || nullptr == rCollider || lCollider == rCollider)
				continue;

			if (!lCollider->Is_Active() || !rCollider->Is_Active())
				continue;

			// gen map key by using union
			COLLIDER_ID ID;
			ID.iLeft_id = lCollider->Get_Id();
			ID.iRight_id = rCollider->Get_Id();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(lCollider, rCollider))
			{
				if (iter->second)
				{
					if (!lCollider->GetOwner()->Is_Active() || !rCollider->GetOwner()->Is_Active())
					{
						lCollider->OnCollisionExit(rCollider);
						rCollider->OnCollisionExit(lCollider);
						iter->second = false;
					}
					else
					{
						lCollider->OnCollisionStay(rCollider);
						rCollider->OnCollisionStay(lCollider);
					}
				}
				else
				{
					if (lCollider->GetOwner()->Is_Active() && rCollider->GetOwner()->Is_Active())
					{
						lCollider->OnCollisionEnter(rCollider);
						rCollider->OnCollisionEnter(lCollider);
						iter->second = true;
					}
				}
			}
			else
			{
				if (iter->second)
				{
					lCollider->OnCollisionExit(rCollider);
					rCollider->OnCollisionExit(lCollider);
					iter->second = false;
				}
			}
		}
	}

	for (_uint i = 0; i < (_uint)COLLISION_GROUP::COLLIDE_END; ++i)
		m_vecCol[i].clear();
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	const _vec3& vLeftScale = ((CBoxCollider*)_pLeftCol)->Get_Scale();
	const _vec3& vLeftPos =	((CBoxCollider*)_pLeftCol)->Get_Pos();
	const _vec3& vRightScale = ((CBoxCollider*)_pRightCol)->Get_Scale();
	const _vec3& vRightPos =	((CBoxCollider*)_pRightCol)->Get_Pos();

	if (fabs(vRightPos.x - vLeftPos.x) < 0.5f * (vLeftScale.x + vRightScale.x)
		&& fabs(vRightPos.y - vLeftPos.y) < 0.5f * (vLeftScale.y + vRightScale.y)
		&& fabs(vRightPos.z - vLeftPos.z) < 0.5f * (vLeftScale.z + vRightScale.z))
	{
		float colX = 0.5f * (vLeftScale.x + vRightScale.x) - fabs(vRightPos.x - vLeftPos.x);
		float colY = 0.5f * (vLeftScale.y + vRightScale.y) - fabs(vRightPos.y - vLeftPos.y);
		float colZ = 0.5f * (vLeftScale.z + vRightScale.z) - fabs(vRightPos.z - vLeftPos.z);

		if (colX < colY && colX < colZ)
			return true;

		else if (colY < colX && colY < colZ)
			return true;

		else if (colZ < colX && colZ < colY)
			return true;
	}

	return false;
}


void CCollisionMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);
}


