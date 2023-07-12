#include "Grass.h"
#include    "Export_Function.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pGr) 
    : CFieldObject(pGr, OBJ_ID::GRASS){}

CGrass::CGrass(const CGrass& rhs) : CFieldObject(rhs), m_vecGrassPos(rhs.m_vecGrassPos), m_vecGrassSize(rhs.m_vecGrassSize)
{
}

CGrass::~CGrass()
{
}

HRESULT CGrass::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());



    return S_OK;
}

_int CGrass::Update_Object(const _float& fTimeDelta)
{
    return _int();
}

void CGrass::LateUpdate_Object(void)
{
}

void CGrass::Render_Object(void)
{
}

void CGrass::Add_Subscribe(_uint iNum)
{
    Engine::Add_Subscribe(iNum, this);
}

void CGrass::Free()
{
}

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
    CGrass* ret = new CGrass(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Grass Create Failed");
        return nullptr;
    }
    ret->m_pTransformCom->Set_Pos(&p_Pos);

    return ret;
}

void CGrass::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CGrass::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CGrass::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CGrass::Event_Start(_uint iEventNum)
{
}

void CGrass::Event_End(_uint iEventNum)
{
}
