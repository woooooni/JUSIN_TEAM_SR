#include "BossDoorEnter.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"
#include "Nexus.h"
#include "GameMgr.h"
#include "BossDoor.h"
#include "GameMgr.h"

CBossDoorEnter::CBossDoorEnter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
{

}

CBossDoorEnter::CBossDoorEnter(const CBossDoorEnter& rhs)
	: CGameObject(rhs)
{
}

CBossDoorEnter::~CBossDoorEnter()
{
}

HRESULT CBossDoorEnter::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(1.0f, 1.0f, 2.0f));



	return S_OK;
}

_int CBossDoorEnter::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PORTAL);
	__super::Update_Object(fTimeDelta);


	return S_OK;
}

void CBossDoorEnter::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBossDoorEnter::Render_Object(void)
{
	__super::Render_Object();
}

void CBossDoorEnter::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::PLAYER)
	{
		CGameObject* pLeftDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"LeftDoor");
		NULL_CHECK_RETURN(pLeftDoor, );
		dynamic_cast<CBossDoor*>(pLeftDoor)->Open_Door();

		CGameObject* pRightDoor = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"RightDoor");
		NULL_CHECK_RETURN(pRightDoor, );
		dynamic_cast<CBossDoor*>(pRightDoor)->Open_Door();

		CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
		NULL_CHECK_RETURN(pPlayer, );
		pPlayer->SetObj_Dir(OBJ_DIR::DIR_U);
		dynamic_cast<CPlayer*>(pPlayer)->Set_Stop(true);
	}
}

void CBossDoorEnter::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBossDoorEnter::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CBossDoorEnter::Ready_Component(void)
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


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

CBossDoorEnter* CBossDoorEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossDoorEnter* pInstance = new CBossDoorEnter(pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossDoorEnter Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CBossDoorEnter::Free()
{
	__super::Free();
}
