#include "BarrelBomb.h"
#include    "Export_Function.h"
#include "Monster.h"
#include    "Player.h"

CBarrelBomb::CBarrelBomb(LPDIRECT3DDEVICE9 p_Dev)
    : CFieldObject(p_Dev, OBJ_ID::JELLY_BOMB)
    , m_bHitted(false), m_fExplodeTime(0.f), m_iExplodeEvent(0), m_bExplosing(false)
    , m_fBlurAlpha(0.f)
    ,m_bIsPlusAlpha(false)
    ,m_fExistTime (0.f)
{
}

CBarrelBomb::CBarrelBomb(const CBarrelBomb& rhs)
    : CFieldObject(rhs), m_bHitted(rhs.m_bHitted), m_fExplodeTime(rhs.m_fExplodeTime), m_iExplodeEvent(rhs.m_iExplodeEvent)
    , m_fBlurAlpha(rhs.m_fBlurAlpha)
    , m_bExplosing(rhs.m_bExplosing)
    ,m_bIsPlusAlpha(rhs.m_bIsPlusAlpha)
    ,m_fExistTime(rhs.m_fExistTime)
{
    m_tInfo.m_bIsAttackable = true;
    m_tInfo.m_bIsPushable = true;
    Set_MinHeight(0.5f);

}

CBarrelBomb::~CBarrelBomb()
{
}

HRESULT CBarrelBomb::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_Barrel_Explosive", 1.f);
    m_pAnimator->Add_Animation(L"Explode", L"Proto_Tex_JellyBomb_Explode", 0.1f);
    m_pAnimator->Play_Animation(L"Base", false);

    CComponent* pCOm = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Barrel_Explosive_Blur"));
    NULL_CHECK_RETURN(pCOm, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pCOm);

    return S_OK;
}

_int CBarrelBomb::Update_Object(const _float& fTimeDelta)
{
    if (m_bHitted)
    {
        if (m_fBlurAlpha < 1.f && m_bIsPlusAlpha)
        {
            m_fBlurAlpha += fTimeDelta * 10;
        }
        else if (m_fBlurAlpha >= 1.f && m_bIsPlusAlpha)
        {
            m_fBlurAlpha = 1.f;
            m_bIsPlusAlpha = false;
        }
        else if (m_fBlurAlpha > 0.f && !m_bIsPlusAlpha)
        {
            m_fBlurAlpha -= fTimeDelta * 10;
        }
        else if (m_fBlurAlpha <= 0.f && !m_bIsPlusAlpha)
        {
            m_fBlurAlpha = 0.f;
            m_bIsPlusAlpha = true;
        }

        m_fExplodeTime += fTimeDelta;

    }
    if ((m_fExplodeTime > 2.f || m_fExistTime > 5.f) && !m_bExplosing)
    {
        m_bHitted = false;
        m_pAnimator->Play_Animation(L"Explode", false);
        m_bExplosing = true;
        m_fExplodeTime = 0.f;
        m_fBlurAlpha = 0.f;
        m_bIsPlusAlpha = false;
        dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 5.f, 5.f, 5.f });
        m_pTransformCom->Set_Scale({ 5.f, 5.f, 5.f });
        _vec3 mypos;
        m_pTransformCom->Get_Info(INFO_POS, &mypos);

        mypos.y = 0.02f;
        m_pTransformCom->Set_Pos(&mypos);
        m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));

    }
    else if (m_fExistTime >= 3.f)
    {
        m_bHitted = true;
    }


    if (m_bExplosing)
    {
        m_fExplodeTime += fTimeDelta;

        if (m_pAnimator->GetCurrAnimation()->Get_Idx() == (m_pAnimator->GetCurrAnimation()->Get_Size() - 1))
        {
            Set_Active(false);
            return 0;
        }

        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOMB);
        
    }
    else
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

    }



    Add_RenderGroup(RENDER_ALPHA, this);
    m_fExistTime += fTimeDelta;

    return __super::Update_Object(fTimeDelta);
}

void CBarrelBomb::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CBarrelBomb::Render_Object(void)
{
    _matrix world = *m_pTransformCom->Get_WorldMatrix();

    
    __super::Render_Object();

    if (m_bHitted && !m_bExplosing)
    {
        world._43 -= 0.01f;
    
        LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

        CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
        if (pCamera == nullptr)
            return;

        _vec3 vPos;
        pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
        D3DVECTOR vCamPos = vPos;

        D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_fBlurAlpha };

        pEffect->SetMatrix("g_WorldMatrix", &world);
        pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
        pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
        pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
        pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
        pEffect->SetFloat("g_AlphaRef", 50.0f);


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

void CBarrelBomb::Free()
{
    __super::Free();
}

CBarrelBomb* CBarrelBomb::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos)
{
    CBarrelBomb* ret = new CBarrelBomb(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("CBarrelBomb Failed");
        return nullptr;
    }

    ret->m_pTransformCom->Set_Pos(&p_Pos);
    ret->Set_MinHeight(0.5f);

    return ret;
}

void CBarrelBomb::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && !m_bHitted && !m_bExplosing)
    {
        m_bHitted = true;
        m_bIsPlusAlpha = true;

    }
}

void CBarrelBomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (!m_bExplosing &&  _eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
        || !m_bExplosing && _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER
        || _eCollisionGroup == COLLISION_GROUP::COLLIDE_ITEM
        || (!m_bExplosing && _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
        || (!m_bExplosing && _eCollisionGroup == COLLISION_GROUP::COLLIDE_BREAK))
    {
        _float fDeltaX = 0.f, fDeltaY = 0.f, fDeltaZ = 0.f;

        _vec3 vPos, vOtherPos;
        _vec3 vMin, vMax;
        _vec3 vOtherMin, vOtherMax;

        CTransform* pTransform = m_pTransformCom;
        CTransform* pOtherTransform = pCollider->GetOwner()->Get_TransformCom();

        CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
        CBoxCollider* pOtherBoxCollider = dynamic_cast<CBoxCollider*>(pCollider);


        NULL_CHECK_RETURN(pTransform, );
        NULL_CHECK_RETURN(pOtherTransform, );
        NULL_CHECK_RETURN(pBoxCollider, );
        NULL_CHECK_RETURN(pOtherBoxCollider, );

        pTransform->Get_Info(INFO_POS, &vPos);
        pOtherTransform->Get_Info(INFO_POS, &vOtherPos);

        _vec3 vDir = vOtherPos - vPos;
        _float fX = (pBoxCollider->Get_Scale().x * 0.5f) + (pOtherBoxCollider->Get_Scale().x * 0.5f);
        _float fY = (pBoxCollider->Get_Scale().y * 0.5f) + (pOtherBoxCollider->Get_Scale().y * 0.5f);
        _float fZ = (pBoxCollider->Get_Scale().z * 0.5f) + (pOtherBoxCollider->Get_Scale().z * 0.5f);


        if (fabs(vDir.x) >= fX)
            return;

        if (fabs(vDir.y) >= fY)
            return;

        if (fabs(vDir.z) >= fZ)
            return;



        if (fX - fabs(vDir.x) <= fZ - fabs(vDir.z))
        {
            if (vDir.x < 0.f)
            {
                vDir.x -= (fX - fabs(vDir.x));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
            else
            {
                vDir.x += (fX - fabs(vDir.x));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
        }
        /*else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
        {
            if (vDir.y < 0.f)
            {
                vDir.y -= (fY - fabs(vDir.y));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
            else
            {
                vDir.y += (fY - fabs(vDir.y));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
        }*/
        else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x))
        {
            if (vDir.z < 0.f)
            {
                vDir.z -= (fZ - fabs(vDir.z));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
            else
            {
                vDir.z += (fZ - fabs(vDir.z));
                vOtherPos = vPos + vDir;
                pOtherTransform->Set_Pos(&vOtherPos);
            }
        }


    }


}

void CBarrelBomb::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBarrelBomb::Event_Start(_uint iEventNum)
{
}

void CBarrelBomb::Event_End(_uint iEventNum)
{
}

void CBarrelBomb::Set_SubscribeEvent(_uint pEvent)
{
}

void CBarrelBomb::Reset()
{
}
