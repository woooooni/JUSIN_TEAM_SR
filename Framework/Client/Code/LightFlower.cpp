#include "LightFlower.h"
#include    "Export_Function.h"
#include    "BalpanObj.h"
#include    "RollingBug.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev) 
    : CFieldObject(pGraphicDev, OBJ_ID::LIGHT_FLOWER)
    , m_bIsOpened(false)
    , m_eColor(JELLY_COLOR::JELLY_END)
    , m_pBalPan(nullptr)
    ,m_eType(LIGHT_TYPE::LIGHT_END)
{
    _matrix scale, rot;

    D3DXMatrixScaling(&scale, 10.f, 10.f, 1.f); //여기서 바꾸기
    D3DXMatrixRotationX(&rot, D3DXToRadian(90.f));

    AreaWorld = scale * rot;
}

CLightFlower::CLightFlower(const CLightFlower& rhs) : CFieldObject(rhs), AreaWorld(rhs.AreaWorld), m_bIsOpened(rhs.m_bIsOpened), m_eColor(rhs.m_eColor), m_pBalPan(rhs.m_pBalPan), m_eType(rhs.m_eType)
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

    //Engine::Ready_Light(m_pGraphicDev, &tLight, 1);


    return S_OK;
}

_int CLightFlower::Update_Object(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    return __super::Update_Object(fTimeDelta);
}

void CLightFlower::LateUpdate_Object(void)
{
    if (m_bIsOpened && m_pAnimator->GetCurrAnimation()->Get_Idx() == m_pAnimator->GetCurrAnimation()->Get_Size() - 1)
    {
        if (!CLightMgr::GetInstance()->Get_Light(m_eType)->Is_LightOn())
        {
            CLightMgr::GetInstance()->Get_Light(m_eType)->Set_LightOn();
        }
    }

        _vec3 myPos, opPos;
        m_pTransformCom->Get_Info(INFO_POS, &myPos);
        myPos.y = 0;
        CRollingBug* tmp;

        for (auto& iter : Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec())
        {
            if ((tmp = dynamic_cast<CRollingBug*>(iter)) && tmp->Get_Color() == m_eColor && CLightMgr::GetInstance()->Get_Light(m_eType)->Is_LightOn())
            {
                tmp->Get_TransformCom()->Get_Info(INFO_POS, &opPos);
                opPos.y = 0;

                if (D3DXVec3Length(&(opPos - myPos)) < 4.f)
                    tmp->Set_Stun(0.1f);
                else
                {
                    if (tmp->Get_State() == MONSTER_STATE::STUN)
                    {
                        tmp->Set_Stun(0.f);
                        tmp->Set_State(MONSTER_STATE::IDLE);

                    }
                }


            }
       }
    __super::LateUpdate_Object();
}

void CLightFlower::Render_Object(void)
{
    const _matrix* world = m_pTransformCom->Get_WorldMatrix();

    D3DCOLORVALUE   areaColor;

    if (m_eColor != JELLY_COLOR::JELLY_END)
    {
        switch (m_eColor)
        {
        case Engine::JELLY_COLOR::CYAN:
            areaColor = { 0, 1.0f, 1.0f, 1.0f };
            break;
        case Engine::JELLY_COLOR::MAGENTA:
            areaColor = { 1.0f, 0, 1.0f, 1.0f };
            break;
        case Engine::JELLY_COLOR::YELLOW:
            areaColor = { 1.0f, 1.0f, 0, 1.0f };
            break;
        case Engine::JELLY_COLOR::RED:
            areaColor = { 1.0f, 0, 0, 1.0f };
            break;
        case Engine::JELLY_COLOR::BLUE:
            areaColor = { 0, 0, 1.0f, 1.0f };
            break;
        case Engine::JELLY_COLOR::GREEN:
            areaColor = { 0, 1.0f, 0, 1.0f };
            break;
        case Engine::JELLY_COLOR::JELLY_END:
        default:
            MSG_BOX("Error");
            return;
        }

    }

    if (m_eType != LIGHT_TYPE::LIGHT_END && m_eColor != JELLY_COLOR::JELLY_END)
    {
        CLight* iter = CLightMgr::GetInstance()->Get_Light(m_eType);
        iter->Get_LightInfo().Diffuse = areaColor;

    }


    if (m_pGraphicDev->LightEnable(1, true) == S_OK)
    {
        int i = 0;
    }

    __super::Render_Object();

    if (m_bIsOpened && m_pAnimator->GetCurrAnimation()->Get_Idx() == m_pAnimator->GetCurrAnimation()->Get_Size() - 1)
    {

        AreaWorld._41 = world->_41;
        AreaWorld._42 = 0.01f;
        AreaWorld._43 = world->_43;



        LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

        CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
        if (pCamera == nullptr)
            return;

        _vec3 vPos;
        pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
        D3DVECTOR vCamPos = vPos;

        D3DCOLORVALUE vColor = areaColor;

        pEffect->SetMatrix("g_WorldMatrix", &AreaWorld);
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
        pEffect->SetFloat("g_AlphaRef", 0.0f);


        IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
        pEffect->SetTexture("g_Texture", pTexture);


        CLightMgr::GetInstance()->Set_LightToEffect(pEffect);


        pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

        pEffect->Begin(nullptr, 0);
        pEffect->BeginPass(1);

        m_pBufferCom->Render_Buffer();

        pEffect->EndPass();
        pEffect->End();
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
    ret->m_pColliderCom->Set_Offset(_vec3({ 0.f, -2.f, 0.f }));
    ret->m_pTransformCom->Set_Pos(&(p_Pos));
    ret->Add_Subscribe(p_EventNum);
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
        _vec3 myPos, opPos;
        m_pTransformCom->Get_Info(INFO_POS, &myPos);
        myPos.y = 0;
        CRollingBug* tmp;


        for (auto& iter : Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec())
        {
            if ((tmp = dynamic_cast<CRollingBug*>(iter)) && tmp->Get_Color() == m_eColor && CLightMgr::GetInstance()->Get_Light(m_eType)->Is_LightOn())
            {
                tmp->Get_TransformCom()->Get_Info(INFO_POS, &opPos);
                opPos.y = 0;

                if (D3DXVec3Length(&(opPos - myPos)) < 4.f)
                {
                    tmp->Set_Stun(0.f);
                    tmp->Set_State(MONSTER_STATE::IDLE);

                }
            }
        }


        m_pAnimator->Play_Animation(L"Close", false);
        m_eColor = JELLY_COLOR::JELLY_END;
        m_bIsOpened = false;

        if (m_eType != LIGHT_TYPE::LIGHT_END)
        {
            CLightMgr::GetInstance()->Get_Light(m_eType)->Set_LightOff();
        }



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

void CLightFlower::Set_LightType(LIGHT_TYPE _eType)
{
    m_eType = _eType;
    _vec3 pos;
    m_pTransformCom->Get_Info(INFO_POS, &pos);
    CLightMgr::GetInstance()->Get_Light(m_eType)->Get_LightInfo().Position = { pos.x , 3.5f, pos.z - 0.01f };
}
