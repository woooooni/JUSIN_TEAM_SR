#include "Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType, OBJ_ID _eID)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ITEM, _eID)
	, m_eItemType(_eItemType)
	, m_iInvenCount(0)
	, m_eCode(ITEM_CODE::ITEM_END)
	, m_tInfo({ 0.f, 0.f, 85.f, 85.f })
	, m_bIsInInven(false)

{

}

CItem::CItem(const CItem& rhs)
	: Engine::CGameObject(rhs)
	, m_eItemType(rhs.m_eItemType)
	,m_iInvenCount(rhs.m_iInvenCount)
	, m_eCode(rhs.m_eCode)
	, m_bIsInInven(rhs.m_bIsInInven)
	, m_tInfo(rhs.m_tInfo)

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
	if (m_bIsInInven)
		return 0;

	_int iExit = __super::Update_Object(fTimeDelta);

	
	return iExit;
}

void CItem::LateUpdate_Object(void)
{
	if (m_bIsInInven)
		return;
	__super::LateUpdate_Object();
}

void CItem::Render_Object(void)
{
	
	__super::Render_Object();
}

void CItem::Render_UI()
{
	Ready_TransWorld();
	__super::Render_Object();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

_bool CItem::Check_Clicked()
{
	auto& pt = CKeyMgr::GetInstance()->GetMousePos();

	if (pt.x > m_tInfo.fX - m_tInfo.fCX * 0.5f && pt.x < m_tInfo.fX + m_tInfo.fCX * 0.5f
		&& pt.y > m_tInfo.fY - m_tInfo.fCY * 0.5f && pt.y < m_tInfo.fY + m_tInfo.fCY * 0.5f)
	{
		return true;
	}

	return false;
}

CTexture* CItem::Get_Texture()
{
	return m_pTextureCom;
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

wstring CItem::Get_Explain(const ITEM_CODE& pCode)
{
	switch (pCode)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		return L"체력을 조금 회복하는 과일";
		break;
	case Engine::ITEM_CODE::HP_MIDDLE:
		return L"체력을 어느정도 회복하는 과일";

		break;
	case Engine::ITEM_CODE::HP_BIG:
		return L"체력을 많이 회복하는 비싼 과일";

		break;
	case Engine::ITEM_CODE::SPEED_SMALL:
		return L"속도가 조금 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::SPEED_MIDDLE:
		return L"속도가 어느정도 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::SPEED_BIG:
		return L"속도가 많이 올라가는 신비한 약초";

		break;
	case Engine::ITEM_CODE::LEAF:
		return L"흔한 나뭇잎";

		break;
	case Engine::ITEM_CODE::TWIG:
		return L"흔한 나뭇가지";

		break;

	case ITEM_CODE::BUTTERFLY:

		return L"아마도 나비";
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

void CItem::Ready_TransWorld()
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);


	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / matPreProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / matPreProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_vec3 vScale = _vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

}
