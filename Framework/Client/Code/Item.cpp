#include "Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType, OBJ_ID _eID)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ITEM, _eID)
	, m_eItemType(_eItemType)

{

}

CItem::CItem(const CItem& rhs)
	: Engine::CGameObject(rhs)
	, m_eItemType(rhs.m_eItemType)
{

}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	
	return iExit;
}

void CItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_CRcTex"));
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


void CItem::Free()
{
	__super::Free();
}

