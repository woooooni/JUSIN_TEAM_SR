#include "UI_ItemInfo.h"
#include "Export_Function.h"

CUI_ItemInfo::CUI_ItemInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tItemInfo{}
{
}

CUI_ItemInfo::CUI_ItemInfo(const CUI_ItemInfo& rhs)
	: CUI(rhs), m_tItemInfo(rhs.m_tItemInfo)
{
}

CUI_ItemInfo::~CUI_ItemInfo()
{
}

HRESULT CUI_ItemInfo::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fX = 656.f;
	m_tInfo.fY = 372.f;

	m_vDefaultPos = { ((2 * (m_tInfo.fX)) / WINCX - 1.78f) * (1 / m_matProj._11) ,
					((-2 * (m_tInfo.fY)) / WINCY + 1.08f) * (1 / m_matProj._22), 0.f };

	return S_OK;
}

_int CUI_ItemInfo::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_ItemInfo::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_ItemInfo::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3	vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	if (m_bShown)
	{
		switch (m_tItemInfo.eType)
		{
		case SHOPITEMTYPE::UISHOP_CLOTH:
			vPos = { m_vDefaultPos.x, m_vDefaultPos.y, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
			break;

		case SHOPITEMTYPE::UISHOP_BRANCH:
			vPos = { (m_vDefaultPos.x + 148.f), m_vDefaultPos.y, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
			break;

		case SHOPITEMTYPE::UISHOP_LEAF:
			vPos = { (m_vDefaultPos.x + 306.f), m_vDefaultPos.y, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
			break;

		case SHOPITEMTYPE::SHOPKEY_L:
			vPos = { (m_vDefaultPos.x + 630.f), m_vDefaultPos.y + 150.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio * 1.2f, fHeight * fRatio * 1.2f, 0.f);
			break;

		default:
			break;
		}

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUI_ItemInfo::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	switch (m_tItemInfo.eType)
	{
	case SHOPITEMTYPE::UISHOP_BRANCH:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Branch"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::UISHOP_CLOTH:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Cloth"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::UISHOP_LEAF:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Leaf"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOPKEY_L:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_ShortKey_Close"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	default:
		return E_FAIL;
		break;
	}
	return S_OK;
}

void CUI_ItemInfo::Set_Type(SHOPITEMTYPE eType)
{
	m_tItemInfo.eType = eType;
	m_tItemInfo.iPrice = 1;
}

void CUI_ItemInfo::Key_Input()
{
	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
			m_bShown = TRUE;

		else
			m_bShown = FALSE;
	}
}

CUI_ItemInfo* CUI_ItemInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev, SHOPITEMTYPE eType)
{
	CUI_ItemInfo* pInstance = new CUI_ItemInfo(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI ShopItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_ItemInfo::Free()
{
}
