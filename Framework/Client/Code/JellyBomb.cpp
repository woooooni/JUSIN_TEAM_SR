#include "JellyBomb.h"
#include    "Export_Function.h"
#include    "Player.h"

CJellyBomb::CJellyBomb(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev)
{
    m_tInfo.m_bIsAttackable = true;
    m_tInfo.m_bIsGrabbable = true;
    m_tInfo.m_bIsPushable = true;
}

CJellyBomb::CJellyBomb(const CJellyBomb& rhs) : CFieldObject(rhs)
{
}

CJellyBomb::~CJellyBomb()
{
}

HRESULT CJellyBomb::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    return S_OK;
}

_int CJellyBomb::Update_Object(const _float& fTimeDelta)
{



    return __super::Update_Object(fTimeDelta);
}

void CJellyBomb::LateUpdate_Object(void)
{
}

void CJellyBomb::Render_Object(void)
{
}

void CJellyBomb::Free()
{
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

    Add_Subscribe(p_EventNum, ret);
    ret->m_pTransformCom->Set_Pos(&p_Pos);

    return ret;
}

void CJellyBomb::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{
    CPlayer* src = dynamic_cast<CPlayer*>(pCollisionObj);

    if (src)
    {
        if (true)
        {

        }
    }

}

void CJellyBomb::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CJellyBomb::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
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
}
