#include "ClearField.h"
#include    "Export_Function.h"
#include    "PushStone.h"
#include        "Pool.h"

CClearField::CClearField(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::CLEAR_FIELD), m_bCreating(false), m_fFrame(0.f), m_fMaxFrame(0.f)
{
}

CClearField::CClearField(const CClearField& rhs) : CFieldObject(rhs), m_bCreating(rhs.m_bCreating), m_fFrame(0.f), m_fMaxFrame(0.f)
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
            m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 1.f });
        }
        else if (5.f <= m_pAnimator->GetCurrAnimation()->Get_Idx() && m_pAnimator->GetCurrAnimation()->Get_Idx() < 10)
        {
            m_pTransformCom->Set_Scale({ 2.f, 2.f, 1.f });
        }
        else if (10.f <= m_pAnimator->GetCurrAnimation()->Get_Idx() && m_pAnimator->GetCurrAnimation()->Get_Idx() < 15)
        {
            m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
        }

    }
    else if (m_fFrame < m_fMaxFrame)
    {
        m_fFrame += fTimeDelta * 25.f;

        if (m_fFrame < 5.f)
        {
            m_pTransformCom->Set_Scale({ 4.f, 4.f, 1.f });
        }
        else if (5.f <= m_fFrame && m_fFrame < 10.f)
        {
            m_pTransformCom->Set_Scale({ 2.f, 2.f, 1.f });
        }
        else if (10.f <= m_fFrame && m_fFrame < 15.f)
        {
            m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 1.f });
        }

    }

    if (m_fFrame >= 15.f)
    {
        CPool<CClearField>::Return_Obj(this);
        m_fFrame = 0.f;
        m_fMaxFrame = 0.f;
        m_bCreating = true;
        m_pAnimator->Play_Animation(L"Appear", false);

    }
    else
    {
        Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_TRIGGER);
        Add_RenderGroup(RENDER_ALPHA, this);
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
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pTextureCom->Render_Texture((_uint)m_fFrame);
        m_pBufferCom->Render_Buffer();
    }
}

void CClearField::Free()
{
    __super::Free();
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
    
    ret->m_pTransformCom->Set_Pos(&p_Pos);
    ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));

    ret->Set_MinHeight(0.005f);

    return ret;
}

void CClearField::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    CPushStone* src;

    if (m_fMaxFrame >= 15.f)
        return;

    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER)
    {
        //몬스터 죽이는 코드
        m_fMaxFrame += 5.f;

    }
    else if ((_eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH) && (src = dynamic_cast<CPushStone*>(pCollider->GetOwner())))
    {
        //돌에 모자씌우는 코드
    }
}

void CClearField::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CClearField::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
