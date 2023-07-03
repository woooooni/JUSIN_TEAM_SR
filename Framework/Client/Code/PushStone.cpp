#include  "../Include/stdafx.h"
#include "PushStone.h"
#include "Export_Function.h"

CPushStone::CPushStone(LPDIRECT3DDEVICE9 pDev) : CInteractiveObj(pDev)
{
}

CPushStone::CPushStone(const CPushStone& rhs) : CInteractiveObj(rhs)
{
}

CPushStone::~CPushStone()
{
}

HRESULT CPushStone::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

    return S_OK;
}

_int CPushStone::Update_Object(const _float& fTimeDelta)
{
    return _int();
}

void CPushStone::LateUpdate_Object(void)
{
}

void CPushStone::Render_Object(void)
{
}

HRESULT CPushStone::Grap_Obj(CPlayer* p_Owner)
{
    return E_NOTIMPL;
}

HRESULT CPushStone::Fall_Obj()
{
    return E_NOTIMPL;
}

void CPushStone::Push_Obj(const _vec3& pDirection)
{

}

CPushStone* CPushStone::Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPushStone* pInstance = new CPushStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;

}

HRESULT CPushStone::Ready_Component()
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

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;

}


