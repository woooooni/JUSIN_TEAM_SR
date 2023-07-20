#include "Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType, OBJ_ID _eID)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ITEM, _eID)
	, m_eItemType(_eItemType)
	, m_iInvenCount(0)
	, m_eCode(ITEM_CODE::ITEM_END)


{

}

CItem::CItem(const CItem& rhs)
	: Engine::CGameObject(rhs)
	, m_eItemType(rhs.m_eItemType)
	,m_iInvenCount(rhs.m_iInvenCount)
	, m_eCode(rhs.m_eCode)

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

void CItem::Render_UI()
{
	__super::Render_Object();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
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

wstring CItem::Get_ImgName(const ITEM_CODE& pCode)
{
	switch (pCode)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		return L"HP_Small";
		break;
	case Engine::ITEM_CODE::HP_MIDDLE:
		return L"HP_Middle";

		break;
	case Engine::ITEM_CODE::HP_BIG:
		return L"HP_Big";

		break;
	case Engine::ITEM_CODE::SPEED_SMALL:
		return L"Speed_Small";

		break;
	case Engine::ITEM_CODE::SPEED_MIDDLE:
		return L"Speed_Middle";

		break;
	case Engine::ITEM_CODE::SPEED_BIG:
		return L"Speed_Big";

		break;
	case Engine::ITEM_CODE::LEAF:
		return L"Leaf";

		break;
	case Engine::ITEM_CODE::TWIG:
		return L"Twig";

		break;

	case ITEM_CODE::BUTTERFLY:

		return L"Kabuto";
		break;
	case Engine::ITEM_CODE::ITEM_END:
		break;
	default:
		break;
	}

	MSG_BOX("ItemCode Error");

	return L"";
}


void CItem::Free()
{
	__super::Free();
}

