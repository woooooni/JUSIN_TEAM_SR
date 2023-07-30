#include "ClearField.h"
#include    "Export_Function.h"
#include    "PushStone.h"
#include        "Pool.h"
#include       <time.h>

CClearField::CClearField(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::CLEAR_FIELD), m_bCreating(false), m_fFrame(0.f), m_fMaxFrame(0.f), m_fExistTime(0.f)
{
}

CClearField::CClearField(const CClearField& rhs) : CFieldObject(rhs), m_bCreating(rhs.m_bCreating), m_fFrame(0.f), m_fMaxFrame(0.f), m_fExistTime(rhs.m_fExistTime)
{
}

CClearField::~CClearField()
{
}

HRESULT CClearField::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Appear", L"Proto_Tex_ClearArea_Appear", 0.05f);

    m_pAnimator->Play_Animation(L"Appear", false);

    CComponent* pCom = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_ClearArea_DisAppear"));
    NULL_CHECK_RETURN(pCom, E_FAIL);
    pCom->SetOwner(this);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pCom);

    m_bCreating = true;

    return S_OK;
}

_int CClearField::Update_Object(const _float& fTimeDelta)
{
    if (m_bCreating && (m_pAnimator->GetCurrAnimation()->Get_Idx() == (m_pAnimator->GetCurrAnimation()->Get_Size() - 1)))
    {
        m_bCreating = false;       
    }
    else if (m_bCreating)
    {
        if (m_pAnimator->GetCurrAnimation()->Get_Idx() < 5)
        {
            m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.f, 1.f, 1.f });
        }
        else if (5.f <= m_pAnimator->GetCurrAnimation()->Get_Idx() && m_pAnimator->GetCurrAnimation()->Get_Idx() < 10)
        {
            m_pTransformCom->Set_Scale({ 2.f, 2.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 2.f, 2.f, 1.f });
        }
        else if (10.f <= m_pAnimator->GetCurrAnimation()->Get_Idx() && m_pAnimator->GetCurrAnimation()->Get_Idx() < 15)
        {
            m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 4.f, 4.f, 1.f });

        }

    }
    else if (m_fFrame < m_fMaxFrame)
    {
        m_fFrame += fTimeDelta * 25.f;

        if (m_fFrame < 5.f)
        {
            m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 4.f, 4.f, 1.f });
        }
        else if (5.f <= m_fFrame && m_fFrame < 10.f)
        {
            m_pTransformCom->Set_Scale({ 2.f, 2.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 2.f, 2.f, 1.f });

        }
        else if (10.f <= m_fFrame && m_fFrame < 15.f)
        {
            m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
            dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.f, 1.f, 1.f });

        }

    }

    if (m_fFrame >= 15.f || m_fExistTime >= 5.f)
    {
        Engine::DeleteObjEvt(this);
        CPool<CClearField>::Return_Obj(this);
        m_fFrame = 0.f;
        m_fMaxFrame = 0.f;
        m_bCreating = true;
        m_fExistTime = 0.f;
        m_pAnimator->Play_Animation(L"Appear", false);
    }
    else
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_TRIGGER);
        Add_RenderGroup(RENDER_ALPHA, this);
        m_fExistTime += fTimeDelta;
    }

    return __super::Update_Object(fTimeDelta);
}

void CClearField::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CClearField::Render_Object(void)
{
    if(m_bCreating)
        __super::Render_Object();
    else
    {
        LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

        CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
        if (pCamera == nullptr)
            return;

        _vec3 vPos;
        pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
        D3DVECTOR vCamPos = vPos;


        pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetFloat("g_AlphaRef", 0.0f);


        IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture((_uint)m_fFrame);
        pEffect->SetTexture("g_Texture", pTexture);


        CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

        MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
        MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
        MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
        MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
        MATERIAL.material.Power = 0.0f;

        pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

        pEffect->Begin(nullptr, 0);
        pEffect->BeginPass(0);

        m_pBufferCom->Render_Buffer();

        pEffect->EndPass();
        pEffect->End();

    }

    
}

void CClearField::Free()
{
    __super::Free();
}

void CClearField::Reset() 
{ 
    m_bCreating = true; 
    m_fFrame = 0.f;
    m_pAnimator->Play_Animation(L"Appear", false);
    m_fExistTime = 0.f;

}

CClearField* CClearField::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos)
{
    CClearField* ret = new CClearField(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);

    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Clearfield Create Failed");
        return  nullptr;

    }
    srand(unsigned(time(NULL)));



    ret->m_pTransformCom->Set_Pos(&(p_Pos + _vec3(0, (rand() % 200) * 0.0005f + 0.01f , 0)));
    ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));

    ret->Set_MinHeight(0.005f);
    ret->m_pColliderCom->Set_Offset({ 0, 0.5f, 0 });

    return ret;
}

void CClearField::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    CPushStone* src;

    if (m_fMaxFrame >= 15.f)
        return;

    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER)
    {
        m_fMaxFrame += 5.f;

    }
    else if ((_eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH) && (src = dynamic_cast<CPushStone*>(pCollider->GetOwner())))
    {
        src->Set_Clean();
    }
}

void CClearField::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CClearField::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
