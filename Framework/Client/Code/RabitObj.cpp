#include "..\Header\RabitObj.h"
#include    "Export_Function.h"

CRabitObj::CRabitObj(LPDIRECT3DDEVICE9 p_Dev)
    : CFieldObject(p_Dev, OBJ_ID::BENCH)
    , m_bisUp(false)
    ,m_fUpTime(0.f)
    ,m_fMaxHeight(1.f)
    ,m_vRabitPos({0.f, -1.f, 0.f})
{
}

CRabitObj::CRabitObj(const CRabitObj& rhs) 
    : 
    CFieldObject(rhs),
    m_fUpTime(rhs.m_fUpTime)
    ,m_bisUp(rhs.m_bisUp)
    ,m_fMaxHeight(rhs.m_fMaxHeight)
{
}

CRabitObj::~CRabitObj()
{
}

HRESULT CRabitObj::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    m_pAnimator->Add_Animation(L"Up", L"Proto_Tex_Rabit_Up", 0.1f);
    m_pAnimator->Add_Animation(L"Hit", L"Proto_Tex_Rabit_Up", 0.1f);

    CComponent* pCom = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Rabit_Hole"));


    return S_OK;
}

_int CRabitObj::Update_Object(const _float& fTimeDelta)
{
    if (m_bisUp && m_vRabitPos.y < m_fMaxHeight)
    {
        m_vRabitPos.y += fTimeDelta;
        m_fUpTime += fTimeDelta;

    }
    else if (m_bisUp && m_fUpTime > 3.f)
    {
        m_bisUp = false;
        m_vRabitPos.y -= fTimeDelta;
    }
    else if (m_bisUp)
    {
        m_fUpTime += fTimeDelta;

    }
    else if (m_vRabitPos.y < -1.f)
    {
        m_fUpTime = 0.f;
    }


    return __super::Update_Object(fTimeDelta);
}

void CRabitObj::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CRabitObj::Render_Object(void)
{
    if (m_fUpTime > 0.f)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pAnimator->Render_Component();
        m_pBufferCom->Render_Buffer();
    }

    _matrix mat;
    _vec3   myPos;
    m_pTransformCom->Get_Info(INFO_POS, &myPos);
    D3DXMatrixRotationX(&mat, D3DXToRadian(90.f));
    memcpy(&mat.m[3][0], &myPos, sizeof(_vec3));
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
    m_pTextureCom->Render_Texture();
    m_pBufferCom->Render_Buffer();
}

void CRabitObj::Add_Subscribe(_uint iNum)
{
}

void CRabitObj::Free()
{
    __super::Free();
}

CRabitObj* CRabitObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos)
{
    CRabitObj* ret = new CRabitObj(p_Dev);

    NULL_CHECK_RETURN(ret, nullptr);

    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("RabitObj Create Failed");
        return nullptr;
    }

    ret->m_pTransformCom->Set_Pos(&p_Pos);
    ret->Set_MinHeight(0.01f);

    return ret;
}

void CRabitObj::Set_Up() { m_bisUp = true; m_pAnimator->Play_Animation(L"Up", false); }

void CRabitObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && m_bisUp)
    {
        m_bisUp = false;
        m_pAnimator->Play_Animation(L"Hit", false);
    }
}

void CRabitObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CRabitObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
