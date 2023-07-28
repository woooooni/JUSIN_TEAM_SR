#include "DoorEnter.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"
#include "Nexus.h"
#include "GameMgr.h"
#include "Door.h"
#include "GameMgr.h"

CDoorEnter::CDoorEnter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
{

}

CDoorEnter::CDoorEnter(const CDoorEnter& rhs)
	: CGameObject(rhs)
{
}

CDoorEnter::~CDoorEnter()
{
}

HRESULT CDoorEnter::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(1.0f, 1.0f, 2.0f));



	return S_OK;
}

_int CDoorEnter::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PORTAL);
	__super::Update_Object(fTimeDelta);


	return S_OK;
}

void CDoorEnter::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDoorEnter::Render_Object(void)
{
	__super::Render_Object();
}

void CDoorEnter::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::PLAYER)
	{
		CGameObject* pDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Door");
		NULL_CHECK_RETURN(pDoor, );
		dynamic_cast<CDoor*>(pDoor)->Open_Door();


		CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
		NULL_CHECK_RETURN(pPlayer, );
		pPlayer->SetObj_Dir(OBJ_DIR::DIR_U);
		dynamic_cast<CPlayer*>(pPlayer)->Set_Stop(true);
	}
}

void CDoorEnter::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CDoorEnter::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CDoorEnter::Ready_Component(void)
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

CDoorEnter* CDoorEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoorEnter* pInstance = new CDoorEnter(pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("DoorEnter Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CDoorEnter::Free()
{
	__super::Free();
}
