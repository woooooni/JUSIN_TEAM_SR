#include "MonsterSpawner.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"
#include "MothMage.h"
#include "DesertRhino.h"
#include "TrashBig.h"


CMonsterSpawner::CMonsterSpawner(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, OBJ_ID::NEXUS)
{
}

CMonsterSpawner::CMonsterSpawner(const CMonsterSpawner& rhs)
	: CGameObject(rhs)
{
}

CMonsterSpawner::~CMonsterSpawner()
{
}

HRESULT CMonsterSpawner::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_fSpawnTime = 10.0f;
	m_fAccTime = 0.0f;

	m_iMonsterCount = 10;
	m_fSpawnDelay = 0.05;
	m_iAccCount = 0;

	return S_OK;
}

_int CMonsterSpawner::Update_Object(const _float& fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	if (m_bSpawn)
	{
		
		if (m_fAccTime > m_fSpawnTime)
		{
			m_fSpawnDelay -= fTimeDelta;

			if (m_fSpawnDelay < 0.0f)
			{
				if (m_iAccCount < m_iMonsterCount)
				{
					SpawnMonsters();
					m_fSpawnDelay = 0.05f;
					m_iAccCount += 1;
				}
				else
				{
					m_fAccTime = 0.0f;
					m_iAccCount = 0;
				}
			}
			
		}
		else
			m_fAccTime += fTimeDelta;
	}

	return S_OK;
}

void CMonsterSpawner::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CMonsterSpawner::Render_Object(void)
{
	__super::Render_Object();
}

void CMonsterSpawner::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CMonsterSpawner::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CMonsterSpawner::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CMonsterSpawner::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return S_OK;
}

CMonsterSpawner* CMonsterSpawner::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterSpawner* pInstance = new CMonsterSpawner(pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MonsterSpawner Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMonsterSpawner::Free()
{
	__super::Free();
}

void CMonsterSpawner::SpawnMonsters()
{
	_int iMonsterType = rand() % 10;
	CGameObject* pMonster = nullptr;

	if (iMonsterType < 7)
	{
		pMonster = CTrashBig::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pMonster, );
		Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"TrashBig", pMonster);
	}
	else if (iMonsterType == 7 || iMonsterType == 8)
	{
		pMonster = CDesertRhino::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pMonster, );
		Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"DesertRhino", pMonster);
	}
	else
	{
		pMonster = CMothMage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pMonster, );
		Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"MothMage", pMonster);
	}
	dynamic_cast<CMonster*>(pMonster)->Set_DefenceMode(m_pNexus);
	
	_vec3 vPos, vTargetPos, vDir;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pNexus->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	
	vDir = vTargetPos - vPos;

	vDir.y = vPos.y + 5.0f;

	D3DXVec3Normalize(&vDir, &vDir);

	_float m_fAngle = (rand() % 37) * 10.0f;

	m_fAngle = D3DXToRadian(m_fAngle);

	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &_vec3(0.0f, 1.0f, 0.0f), m_fAngle);
	D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

	pMonster->Get_TransformCom()->Set_Pos(&vPos);
	pMonster->Get_RigidBodyCom()->AddForce(vDir * 70.0f);
}
