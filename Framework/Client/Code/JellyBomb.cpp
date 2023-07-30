#include "JellyBomb.h"
#include    "Export_Function.h"
#include    "Player.h"

CJellyBomb::CJellyBomb(LPDIRECT3DDEVICE9 p_Dev) : 
    CFieldObject(p_Dev, OBJ_ID::JELLY_BOMB), m_pBlurTex(nullptr), m_bHitted(false), m_fExplodeTime(0.f), m_iExplodeEvent(0), m_bExplosing(false)
    , m_fBlurAlpha(0.f)
                    
{
    m_tInfo.m_bIsAttackable = true;
    m_tInfo.m_bIsGrabbable = true;
    m_tInfo.m_bIsPushable = true;
}

CJellyBomb::CJellyBomb(const CJellyBomb& rhs) : CFieldObject(rhs), m_pBlurTex(rhs.m_pBlurTex), m_bHitted(rhs.m_bHitted), m_fExplodeTime(rhs.m_fExplodeTime), m_iExplodeEvent(rhs.m_iExplodeEvent)
                                               , m_fBlurAlpha(rhs.m_fBlurAlpha)
                                                , m_bExplosing(rhs.m_bExplosing)
{
}

CJellyBomb::~CJellyBomb()
{
}

HRESULT CJellyBomb::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());



    m_pAnimator->Add_Animation(L"Effect", L"Proto_Tex_JellyBomb_Effect", 0.1f);
    m_pAnimator->Add_Animation(L"Explode", L"Proto_Tex_JellyBomb_Explode", 0.1f);

    CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_JellyBomb"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE, pComponent });

    pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Clone_Proto(L"Proto_RigidBody"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ COMPONENT_TYPE::COM_RIGIDBODY, pComponent });

    Set_MinHeight(0.5f);

    m_pBlurTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_JellyBomb_Blur"));
    NULL_CHECK_RETURN(m_pBlurTex, E_FAIL);
    m_pBlurTex->SetOwner(this);

    m_pAnimator->Play_Animation(L"Effect", true);

    return S_OK;
}

_int CJellyBomb::Update_Object(const _float& fTimeDelta)
{
    if (!m_bExplosing)
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);
    }
    else
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOMB);
    }

    Add_RenderGroup(RENDER_ALPHA, this);


    if (m_bHitted)
    {
        if (m_fExplodeTime <= 0.f)
        {

            m_bExplosing = true;
            m_bHitted = false;
            m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 4.f, 1.f, 4.f });
            m_pAnimator->Play_Animation(L"Explode", false);
        }
        else
        {
            m_fExplodeTime -= fTimeDelta;
            m_fBlurAlpha += fTimeDelta * 0.3f;

            if (m_fBlurAlpha > 1.f)
                m_fBlurAlpha = 1.f;
        }
    }
    if (m_bExplosing && m_pAnimator->GetCurrAnimation()->Get_Idx() == (m_pAnimator->GetCurrAnimation()->Get_Size() - 1))
    {
        Engine::Check_Event_Start(m_iExplodeEvent);
    }


    return __super::Update_Object(fTimeDelta);
}

void CJellyBomb::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CJellyBomb::Render_Object(void)
{
    _matrix mat = *m_pTransformCom->Get_WorldMatrix();
    _vec3 look, up;

    m_pTransformCom->Get_Info(INFO_LOOK, &look);
    m_pTransformCom->Get_Info(INFO_UP, &up);
    look *= -0.005f;
    up *= 0.05f;


    m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

    if (!m_bExplosing)
    {
        LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

        CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
        if (pCamera == nullptr)
            return;

        _vec3 vPos;
        pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
        D3DVECTOR vCamPos = vPos;


        pEffect->SetMatrix("g_WorldMatrix", &mat);
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetFloat("g_AlphaRef", 0.0f);


        IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
        pEffect->SetTexture("g_Texture", pTexture);


        CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

    

        pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

        pEffect->Begin(nullptr, 0);
        pEffect->BeginPass(0);

        m_pBufferCom->Render_Buffer();

        pEffect->EndPass();
        pEffect->End();

        mat._11 *= 0.7f;
        mat._22 *= 0.7f;
        mat._33 *= 0.7f;


        mat._41 += look.x + up.x;
        mat._42 += look.y + up.y;
        mat._43 += look.z + up.z;


        pEffect->SetMatrix("g_WorldMatrix", &mat);
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetFloat("g_AlphaRef", 0.0f);


        pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
        pEffect->SetTexture("g_Texture", pTexture);


        CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

      

        pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

        pEffect->Begin(nullptr, 0);
        pEffect->BeginPass(0);

        m_pBufferCom->Render_Buffer();

        pEffect->EndPass();
        pEffect->End();

        if (m_bHitted)
        {
            mat._11 /= 0.7f;
            mat._22 /= 0.7f;
            mat._33 /= 0.7f;


            mat._41 += look.x - up.x;
            mat._42 += look.y - up.y;
            mat._43 += look.z - up.z;         

            LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

            CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
            if (pCamera == nullptr)
                return;

            _vec3 vPos;
            pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
            D3DVECTOR vCamPos = vPos;

            D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, (255.f * m_fBlurAlpha) / 255.f };

            pEffect->SetMatrix("g_WorldMatrix", &mat);
            pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
            pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
            pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
            pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
            pEffect->SetFloat("g_AlphaRef", 0.0f);


            IDirect3DBaseTexture9* pTexture = m_pBlurTex->Get_Texture(0);
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
    else
    {
        LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

        CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
        if (pCamera == nullptr)
            return;

        _vec3 vPos;
        pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
        D3DVECTOR vCamPos = vPos;

        D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, (255.f * m_fBlurAlpha) / 255.f };

        pEffect->SetMatrix("g_WorldMatrix", &mat);
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
        pEffect->SetFloat("g_AlphaRef", 0.0f);


        IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
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

void CJellyBomb::Free()
{
    __super::Free();
}

CJellyBomb* CJellyBomb::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3& p_Pos)
{
    CJellyBomb* ret = new CJellyBomb(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Jellybomb Create Failed");
        return nullptr;
    }
    ret->m_iExplodeEvent = p_EventNum;
    ret->m_pTransformCom->Set_Pos(&p_Pos);

    return ret;
}

void CJellyBomb::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && !m_bHitted)
    {
        m_bHitted = true;
        m_fExplodeTime = 3.f;
        Stop_Sound(CHANNELID::SOUND_EFFECT_INTERACTION);
        Play_Sound(L"SFX_88_JellyBomb_Activation.wav", CHANNELID::SOUND_EFFECT_INTERACTION, .5f);
    }

}

void CJellyBomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
        Push_Me(pCollider);
}

void CJellyBomb::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyBomb::Event_Start(_uint iEventNum)
{
}

void CJellyBomb::Event_End(_uint iEventNum)
{
}

void CJellyBomb::Set_SubscribeEvent(_uint pEvent)
{
    Engine::Add_Subscribe(pEvent, this);
}

void CJellyBomb::Reset()
{
    m_bHitted = false;
    m_fExplodeTime = 0.f;
    m_fBlurAlpha = 0.f;
    m_bExplosing = false;
    m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
    dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.f, 1.f, 1.f });
    m_pAnimator->Play_Animation(L"Effect", true);

    Set_Active(true);
}
