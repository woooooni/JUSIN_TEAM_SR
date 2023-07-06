#include "stdafx.h"
#include "BalpanObj.h"
#include "Export_Function.h"

CBalpanObj::CBalpanObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev)
{
}

CBalpanObj::CBalpanObj(const CBalpanObj& rhs) : CFieldObject(rhs)
{
}

CBalpanObj::~CBalpanObj()
{
}

HRESULT CBalpanObj::Ready_Object(void)
{
    return E_NOTIMPL;
}

_int CBalpanObj::Update_Object(const _float& fTimeDelta)
{

    _int       ret = __super::Update_Object(fTimeDelta);
    return ret;
}

void CBalpanObj::LateUpdate_Object(void)
{
}

void CBalpanObj::Render_Object(void)
{
}

void CBalpanObj::Free()
{
    __super::Free();
}

CBalpanObj* CBalpanObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos)
{
    CBalpanObj* ret = new CBalpanObj(p_Dev);

    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Balpan Create Failed");
        return nullptr;
    }


    return ret;
}

HRESULT CBalpanObj::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);


	return S_OK;

}
