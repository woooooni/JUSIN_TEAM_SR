#include "DefenceEnter.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"
#include "Nexus.h"
#include "GameMgr.h"

CDefenceEnter::CDefenceEnter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
{

}

CDefenceEnter::CDefenceEnter(const CDefenceEnter& rhs)
	: CGameObject(rhs)
{
}

CDefenceEnter::~CDefenceEnter()
{
}

HRESULT CDefenceEnter::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(20.0f, 5.0f, 2.0f));

	m_fCameraTime = 5.0f;
	m_fAccTime = 0.0f;

	return S_OK;
}

_int CDefenceEnter::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PORTAL);
	__super::Update_Object(fTimeDelta);

	if (m_bStart)
	{
		m_fAccTime += fTimeDelta;

		if (m_fCameraTime < m_fAccTime)
		{
			CGameObject* pNexus = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Nexus");
			if (pNexus)
			{
				dynamic_cast<CNexus*>(pNexus)->Start_Defence();
				CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
				CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");
				if (pCamera)
				{
					dynamic_cast<CCamera*>(pCamera)->Set_TargetObj(pPlayer);
					dynamic_cast<CCamera*>(pCamera)->Minus_Offset();
				}
			}
			Set_Active(false);
		}
	}

	return S_OK;
}

void CDefenceEnter::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDefenceEnter::Render_Object(void)
{
	__super::Render_Object();
}

void CDefenceEnter::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::PLAYER)
	{
		CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");
		if (pCamera)
		{
			CGameObject* pNexus = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Nexus");
			dynamic_cast<CCamera*>(pCamera)->Set_TargetObj(pNexus);
			dynamic_cast<CCamera*>(pCamera)->Add_Offset();
			m_bStart = true;
		}
	}
}

void CDefenceEnter::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CDefenceEnter::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CDefenceEnter::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	return S_OK;
}

CDefenceEnter* CDefenceEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefenceEnter* pInstance = new CDefenceEnter(pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("DefenceEnter Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CDefenceEnter::Free()
{
	__super::Free();
}
