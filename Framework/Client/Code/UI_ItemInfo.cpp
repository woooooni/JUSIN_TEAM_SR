#include "UI_ItemInfo.h"
#include "Export_Function.h"
#include "UI_Shop.h"
#include "GameMgr.h"
#include "UI_Cursor.h"
#include "UIMgr.h"
#include	"InventoryMgr.h"

CUI_ItemInfo::CUI_ItemInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tItemInfo{}
	,m_pItem(nullptr)
{
}

CUI_ItemInfo::CUI_ItemInfo(const CUI_ItemInfo& rhs)
	: CUI(rhs), m_tItemInfo(rhs.m_tItemInfo)
	, m_pItem(rhs.m_pItem)
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
	if (m_pTextureCom)
	{
		m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	}



	return S_OK;
}

_int CUI_ItemInfo::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	Key_Input();

	return iExit;
}

void CUI_ItemInfo::LateUpdate_Object(void)
{
	CUI_Shop* pShop = CUIMgr::GetInstance()->Get_ShopUI();
	CUI_Cursor* pCursor = pShop->Get_Cursor();

	m_iCursorX = dynamic_cast<CUI_Cursor*>(pCursor)->Get_CursorXPos();
	m_iCursorY = dynamic_cast<CUI_Cursor*>(pCursor)->Get_CursorYPos();

	__super::LateUpdate_Object();
}

void CUI_ItemInfo::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3 vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	fRatio = _float(WINCY) / _float(WINCX);

	vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
			(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	if (m_bShown)
	{
		switch (m_tItemInfo.eType)
		{
		case SHOPITEMTYPE::SHOP_WALLET:
			vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOP_VERLINE:
			vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio * 1.7f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOPKEY_L:
			vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOP_IMGBOX:
			vScale = _vec3(m_tInfo.fCX * fRatio * 0.9f, m_tInfo.fCY * fRatio * 0.8f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOP_TEXTBOX:
			vScale = _vec3(m_tInfo.fCX * fRatio * 0.9f, m_tInfo.fCY * fRatio * 1.2f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOP_HORLINE:
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.4f, m_tInfo.fCY * fRatio * 1.2f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;

		case SHOPITEMTYPE::SHOP_PRICETAG:
 			vScale = _vec3(m_tInfo.fCX * fRatio * 1.1f, m_tInfo.fCY * fRatio * 1.1f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;


		default:
			vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			break;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
		if (m_tItemInfo.eType < SHOPITEMTYPE::UISHOP_LEAF)
		{
			m_pTextureCom->Render_Texture();
			m_pBufferCom->Render_Buffer();

		}

		// Player Money TextOut //
		CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();;

		if (pPlayer != nullptr)
		{
			m_iPlayerMoney = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMoney;
		}

		RECT rc = { 1110, 25, WINCX, WINCY / 2 };

		string strMoney = to_string(m_iPlayerMoney);
		wstring sTemp = wstring(strMoney.begin(), strMoney.end());
		LPCWSTR swBuffer = sTemp.c_str();

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			swBuffer, lstrlen(swBuffer), &rc, DT_CENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		if (m_pItem == nullptr)
			return;
		if (m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_LEAF || m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_CLOTH || m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_BRANCH)
		{
			vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);

			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.f, m_tInfo.fCY * fRatio * 1.f, 0.2f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			//			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
			//			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

			m_pItem->Get_TextureCom()->Render_Texture();
			m_pBufferCom->Render_Buffer();

		}
		else if ((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_CLOTH_INFO)
			&& m_iCursorX == 1 && m_iCursorY == 0)
		{
			vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);

			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.7f, m_tInfo.fCY * fRatio * 1.7f, 0.2f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
//			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
//			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

			m_pItem->Get_TextureCom()->Render_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if ((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_LEAF_INFO )
			&& m_iCursorX == 0 && m_iCursorY == 0)
		{
			vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);

			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.8f, m_tInfo.fCY * fRatio * 1.8f, 0.f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

			m_pItem->Get_TextureCom()->Render_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if ((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_BRANCH_INFO )
			&& m_iCursorX == 2 && m_iCursorY == 0)
		{
			vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);

			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.6f, m_tInfo.fCY * fRatio * 1.6f, 0.05f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

			m_pItem->Get_TextureCom()->Render_Texture();
			m_pBufferCom->Render_Buffer();
		}
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
		m_tInfo.fX = WINCX / 2 - 490.f + 2 * 154.f;
		m_tInfo.fY = WINCY / 2 - 83.f;
		break;

	case SHOPITEMTYPE::UISHOP_CLOTH:

		m_tInfo.fX = WINCX / 2 - 490.f + 154.f;
		m_tInfo.fY = WINCY / 2 - 83.f;
		break;

	case SHOPITEMTYPE::UISHOP_LEAF:

		m_tInfo.fX = WINCX / 2 - 490.f;
		m_tInfo.fY = WINCY / 2 - 83.f;
		break;

	case SHOPITEMTYPE::UISHOP_BRANCH_INFO:
	case SHOPITEMTYPE::UISHOP_CLOTH_INFO:
	case SHOPITEMTYPE::UISHOP_LEAF_INFO:

		m_tInfo.fX = WINCX / 2 + 425.f;
		m_tInfo.fY = WINCY / 2 - 160.f;
		break;


	case SHOPITEMTYPE::SHOPKEY_L:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_ShortKey_Close"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 155.f;
		m_tInfo.fY = WINCY / 2 - 230.f;
		break;

	case SHOPITEMTYPE::SHOP_PRICETAG:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_PriceTag"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 530.f;
		m_tInfo.fY = WINCY / 2 + 15.f;
		break;

	case SHOPITEMTYPE::SHOP_VERLINE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_VerticalLine"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 250.f;
		m_tInfo.fY = WINCY / 2 + 30.f;
		break;

	case SHOPITEMTYPE::SHOP_HORLINE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_HorizontalLine"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 425.f;
		m_tInfo.fY = WINCY / 2 + 50.f;
		break;

	case SHOPITEMTYPE::SHOP_TEXTBOX:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_TextBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 425.f;
		m_tInfo.fY = WINCY / 2 + 156.f;
		break;

	case SHOPITEMTYPE::SHOP_IMGBOX:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_TextBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 425.f;
		m_tInfo.fY = WINCY / 2 - 160.f;
		break;

	case SHOPITEMTYPE::SHOP_WALLET:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_PlayerWallet"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 + 545.f;
		m_tInfo.fY = WINCY / 2 - 390.f;
		break;

	default:
		return E_FAIL;
		break;
	}
	return S_OK;
}

void CUI_ItemInfo::Set_Type(SHOPITEMTYPE eType, const _uint& _ePrice)
{
	m_tItemInfo.eType = eType;
	m_tItemInfo.iPrice = _ePrice;
}

void CUI_ItemInfo::Key_Input()
{
	if (KEY_TAP(KEY::ENTER) && m_pItem != nullptr)
	{

		if (((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_LEAF)
			&& m_iCursorX == 0 && m_iCursorY == 0) || ((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_BRANCH)
				&& m_iCursorX == 2 && m_iCursorY == 0) || ((m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_CLOTH)
					&& m_iCursorX == 1 && m_iCursorY == 0))
		{
			CUI_Shop* pShop = CUIMgr::GetInstance()->Get_ShopUI();
			if (m_iPlayerMoney < m_tItemInfo.iPrice)
			{
				CInventoryMgr::GetInstance()->Add_Item(m_pItem);

				Safe_Release(m_pItem);
				pShop->Remove_Item(m_tItemInfo.eType);
			}
		}

	}
}

HRESULT CUI_ItemInfo::Set_Item(CItem* pItem)
{

	m_pItem = pItem;

	if (m_pItem == nullptr)
		return S_OK;

	m_pItem->Set_UIPos({ m_tInfo.fX, m_tInfo.fY });

	m_pTextureCom = m_pItem->Get_TextureCom();
	m_pTextureCom->AddRef();

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
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
