#include "JellyBomb.h"
#include    "Export_Function.h"
#include    "Player.h"

CJellyBomb::CJellyBomb(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::JELLY_BOMB), m_pBlurTex(nullptr), m_bHitted(false), m_fExplodeTime(0.f), m_iExplodeEvent(0)
{
    m_tInfo.m_bIsAttackable = true;
    m_tInfo.m_bIsGrabbable = true;
    m_tInfo.m_bIsPushable = true;
}

CJellyBomb::CJellyBomb(const CJellyBomb& rhs) : CFieldObject(rhs), m_pBlurTex(rhs.m_pBlurTex), m_bHitted(rhs.m_bHitted), m_fExplodeTime(rhs.m_fExplodeTime), m_iExplodeEvent(rhs.m_iExplodeEvent)
{
}

CJellyBomb::~CJellyBomb()
{
}

HRESULT CJellyBomb::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Effect", L"Proto_Tex_JellyBomb_Effect", 0.1f);

    CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_JellyBomb"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent->insert({ COMPONENT_TYPE::COM_TEXTURE, pComponent });

    m_pBlurTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_JellyBomb_Blur"));
    NULL_CHECK_RETURN(m_pBlurTex, E_FAIL);

    m_pAnimator->Play_Animation(L"Effect", true);

    return S_OK;
}

_int CJellyBomb::Update_Object(const _float& fTimeDelta)
{
    if (Is_Active())
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);
        Add_RenderGroup(RENDER_ALPHA, this);
    }

    if (m_bHitted)
    {
        if (m_fExplodeTime <= 0.f)
        {
            Set_Active(false);
            Engine::Check_Event_Start(m_iExplodeEvent);
        }
        else
            m_fExplodeTime -= fTimeDelta;
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
    m_pTextureCom->Render_Texture();
    m_pBufferCom->Render_Buffer();

    mat._11 *= 0.7f;
    mat._22 *= 0.7f;
    mat._33 *= 0.7f;


    mat._41 += look.x + up.x;
    mat._42 += look.y + up.y;
    mat._43 += look.z + up.z;


    m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
    m_pAnimator->Render_Component();
    m_pBufferCom->Render_Buffer();
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
        m_fExplodeTime = 5.f;
    }

}

void CJellyBomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
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
