#include "LightFlower.h"
#include    "Export_Function.h"
#include    "BalpanObj.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev) 
    : CFieldObject(pGraphicDev, OBJ_ID::LIGHT_FLOWER)
    , m_bIsOpened(false)
    , m_eColor(JELLY_COLOR::JELLY_END)
    , m_pBalPan(nullptr)
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
    pComponent->SetOwner(this);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

    pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Clone_Proto(L"Proto_RigidBody"));
    NULL_CHECK_RETURN_MSG(pComponent, E_FAIL, L"LightArea Create Failed");
    pComponent->SetOwner(this);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);


    m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_LightFlower_Idle", 0.1f);
    m_pAnimator->Add_Animation(L"Idle_Open", L"Proto_Tex_LightFlower_Idle_Open", 0.1f);
    m_pAnimator->Add_Animation(L"Open", L"Proto_Tex_LightFlower_Open", 0.1f);
    m_pAnimator->Add_Animation(L"Close", L"Proto_Tex_LightFlower_Close", 0.1f);

    m_pAnimator->Play_Animation(L"Idle", false);

    m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

    D3DLIGHT9 tLight;
    ZeroMemory(&tLight, sizeof(D3DLIGHT9));

    tLight.Ambient = { 0.4f, 0.4f, 0.4f, 0.4f };
    tLight.Diffuse = { 1.f, 0.f, 0.f, 1.f };
    // tLight.Specular = { 0.f, 1.f, 1.f, 1.f };

    _vec3 vLightPos;
    m_pTransformCom->Get_Info(INFO_POS, &vLightPos);

    vLightPos.y += 1.f;

    tLight.Position = vLightPos;
    tLight.Range = 10.f;
    
    // tLight.Attenuation0 = 0.f;
    // tLight.Attenuation1 = 0.f;

    Engine::Ready_Light(m_pGraphicDev, &tLight, 1);


    return S_OK;
}

_int CLightFlower::Update_Object(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    return __super::Update_Object(fTimeDelta);
}

void CLightFlower::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CLightFlower::Render_Object(void)
{
    const _matrix* world = m_pTransformCom->Get_WorldMatrix();

    if (m_pGraphicDev->LightEnable(1, true) == S_OK)
    {
        int i = 0;
    }

    __super::Render_Object();

    if (m_bIsOpened && m_pAnimator->GetCurrAnimation()->Get_Idx() == m_pAnimator->GetCurrAnimation()->Get_Size() - 1)
    {
        AreaWorld._41 = world->_41;
        AreaWorld._42 = 0.001f;
        AreaWorld._43 = world->_43;

        DWORD   areaColor;

        switch (m_eColor)
        {
        case Engine::JELLY_COLOR::CYAN:
            areaColor = D3DCOLOR_ARGB(255, 0, 255, 255);
            break;
        case Engine::JELLY_COLOR::MAGENTA:
            areaColor = D3DCOLOR_ARGB(255, 255, 0, 255);
            break;
        case Engine::JELLY_COLOR::YELLOW:
            areaColor = D3DCOLOR_ARGB(255, 255, 255, 0);
            break;
        case Engine::JELLY_COLOR::RED:
            areaColor = D3DCOLOR_ARGB(255, 255, 0, 0);
            break;
        case Engine::JELLY_COLOR::BLUE:
            areaColor = D3DCOLOR_ARGB(255, 0, 0, 255);
            break;
        case Engine::JELLY_COLOR::GREEN:
            areaColor = D3DCOLOR_ARGB(255, 0, 255, 0);
            break;
        case Engine::JELLY_COLOR::JELLY_END:
        default:
            MSG_BOX("Error");
            return;
        }

        m_pGraphicDev->SetTransform(D3DTS_WORLD, &AreaWorld);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, areaColor);
        m_pTextureCom->Render_Texture();
        m_pBufferCom->Render_Buffer();
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255,255,255,255));
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
    /*if (dynamic_cast<CBalpanObj*>(p_Balpan) == nullptr)
        return nullptr;*/

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
    ret->m_pTransformCom->Set_Pos(&(p_Pos));

    ret->Set_MinHeight(2.f);


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
