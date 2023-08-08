#include "Portal.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene)
    :CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
    , m_eNextScene(_eNextScene)
{
}

CPortal::CPortal(const CPortal& rhs)
    : CGameObject(rhs)
    , m_eNextScene(rhs.m_eNextScene)
{
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_Object(void)
{
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
    dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(2.0f, 2.0f, 2.0f));

    return S_OK;
}

_int CPortal::Update_Object(const _float& fTimeDelta)
{
    Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PORTAL);
    __super::Update_Object(fTimeDelta);

    return S_OK;
}

void CPortal::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CPortal::Render_Object(void)
{
    __super::Render_Object();
}

void CPortal::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::PLAYER)
    {
        CScene_Loading* pScene = CScene_Loading::Create(m_pGraphicDev, m_eNextScene);
        Engine::Reserve_SceneChange(pScene);
    }
}

void CPortal::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPortal::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CPortal::Ready_Component(void)
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

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene)
{
    CPortal* pInstance = new CPortal(pGraphicDev, _eNextScene);
    NULL_CHECK_RETURN(pInstance, nullptr);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Portal Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CPortal::Free()
{
    __super::Free();
}
