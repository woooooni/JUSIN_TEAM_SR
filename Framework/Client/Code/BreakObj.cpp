#include "BreakObj.h"
#include    "Export_Function.h"
#include    "Pool.h"
#include    "PushStone.h"

CBreakObj::CBreakObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::PUSH_STONE)
, m_bIsBreaking(false)
{
}

CBreakObj::CBreakObj(const CBreakObj& rhs) : CFieldObject(rhs)
, m_bIsBreaking(rhs.m_bIsBreaking)
{
}

CBreakObj::~CBreakObj()
{
}

HRESULT CBreakObj::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_BreakObj_Idle", 0.f);
    m_pAnimator->Add_Animation(L"Break", L"Proto_Tex_BreakObj_Breaking", 0.1f);

    m_pAnimator->Play_Animation(L"Idle", false);

    return S_OK;
}

_int CBreakObj::Update_Object(const _float& fTimeDelta)
{
    if (m_bIsBreaking && m_pAnimator->GetCurrAnimation()->Get_Idx() == m_pAnimator->GetCurrAnimation()->Get_Size() - 1)
    {
        m_bIsBreaking = false;
        m_pAnimator->Play_Animation(L"Idle", false);
        CPool<CBreakObj>::Return_Obj(this);
        return __super::Update_Object(fTimeDelta);;
    }


    Add_RenderGroup(RENDER_ALPHA, this);
    Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

    

    return __super::Update_Object(fTimeDelta);
}

void CBreakObj::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CBreakObj::Render_Object(void)
{
    __super::Render_Object();
}

void CBreakObj::Add_Subscribe(_uint iNum)
{
}

void CBreakObj::Free()
{
    __super::Free();
}

CBreakObj* CBreakObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos)
{
    CBreakObj* ret = new CBreakObj(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("BreakObj Create Failed");
        return nullptr;
    }

    ret->m_pTransformCom->Set_Pos(&p_Pos);
    ret->Set_MinHeight(0.5f);

    return ret;
}

void CBreakObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
    {
        CPushStone* tmp;
        if ((tmp = dynamic_cast<CPushStone*>(pCollider->GetOwner())) && tmp->Is_Flying())
        {
            m_bIsBreaking = true;
            m_pAnimator->Play_Animation(L"Break", false);
        }
    }
}

void CBreakObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BOMB && !m_bIsBreaking)
    {
        m_bIsBreaking = true;
        m_pAnimator->Play_Animation(L"Break", false);
    }
}
