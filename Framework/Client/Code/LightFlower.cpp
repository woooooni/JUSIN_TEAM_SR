#include "LightFlower.h"
#include    "Export_Function.h"
#include    "BalpanObj.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev) : CFieldObject(pGraphicDev), m_bIsOpened(false), m_eColor(JELLY_COLOR::JELLY_END), m_pBalPan(nullptr)
{
    _matrix scale, rot;

    D3DXMatrixScaling(&scale, 10.f, 10.f, 1.f); //여기서 바꾸기
    D3DXMatrixRotationX(&rot, D3DXToRadian(90.f));

    AreaWorld = scale * rot;
}

CLightFlower::CLightFlower(const CLightFlower& rhs) : CFieldObject(rhs), AreaWorld(rhs.AreaWorld), m_bIsOpened(rhs.m_bIsOpened), m_eColor(rhs.m_eColor), m_pBalPan(rhs.m_pBalPan)
{
}

CLightFlower::~CLightFlower()
{
}

HRESULT CLightFlower::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_LightFlower_Area"));
    NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"LightArea Create Failed");
    m_mapComponent->emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);



    m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_LightFlower_Idle", 0.1f);
    m_pAnimator->Add_Animation(L"Idle_Open", L"Proto_Tex_LightFlower_Idle_Open", 0.1f);
    m_pAnimator->Add_Animation(L"Open", L"Proto_Tex_LightFlower_Open", 0.1f);
    m_pAnimator->Add_Animation(L"Close", L"Proto_Tex_LightFlower_Close", 0.1f);

    m_pAnimator->Play_Animation(L"Idle", false);


    return S_OK;
}

_int CLightFlower::Update_Object(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    return __super::Update_Object(fTimeDelta);
}

void CLightFlower::LateUpdate_Object(void)
{
}

void CLightFlower::Render_Object(void)
{
    const _matrix* world = m_pTransformCom->Get_WorldMatrix();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, world);
    __super::Render_Object();
    m_pBufferCom->Render_Buffer();

    if (m_bIsOpened && m_pAnimator->GetCurrAnimation()->Get_Idx() == m_pAnimator->GetCurrAnimation()->Get_Size() - 1)
    {
        AreaWorld._41 = world->_41;
        AreaWorld._42 = 0.001f;
        AreaWorld._43 = world->_43;


        m_pGraphicDev->SetTransform(D3DTS_WORLD, &AreaWorld);
        m_pTextureCom->Render_Texture();
        m_pBufferCom->Render_Buffer();
    }
}

void CLightFlower::Add_Subscribe(_uint iNum)
{
    Engine::Add_Subscribe(iNum, this);
}

void CLightFlower::Free()
{
}

CLightFlower* CLightFlower::Create(LPDIRECT3DDEVICE9 p_Dev, CGameObject* p_Balpan, const _uint& p_EventNum, const _vec3 p_Pos)
{
    if (dynamic_cast<CBalpanObj*>(p_Balpan) == nullptr)
        return nullptr;

    CLightFlower* ret = new CLightFlower(p_Dev);

    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("LightFlowerCreate Failed");
        return nullptr;

    }

    ret->m_pBalPan = dynamic_cast<CBalpanObj*>(p_Balpan);
    ret->m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
    ret->m_pColliderCom->Set_Offset(_vec3({ 0.f, -1.f, 0.f }));
    ret->m_pTransformCom->Set_Pos(&(p_Pos + _vec3({0.f, 1.f, 0.f})));

    return ret;
}

void CLightFlower::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CLightFlower::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

}

void CLightFlower::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CLightFlower::Event_Start(_uint iEventNum)
{
    _uint max = m_pAnimator->GetCurrAnimation()->Get_Size();
    _uint tmp = m_pAnimator->GetCurrAnimation()->Get_Idx();
    if (m_bIsOpened)
    {
        m_pAnimator->Play_Animation(L"Close", false);
        m_eColor = JELLY_COLOR::JELLY_END;
        m_bIsOpened = false;

    }
    else
    {
        m_pAnimator->Play_Animation(L"Open", false);
        m_eColor = m_pBalPan->Get_JellyColor();
        m_bIsOpened = true;
    }

    m_pAnimator->GetCurrAnimation()->Set_Idx(max - tmp - 1);
}

void CLightFlower::Event_End(_uint iEventNum)
{
}
