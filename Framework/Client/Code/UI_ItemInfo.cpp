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

	// 복붙의 잔해,, WINCX 1280 WINCY 860 기준으로 다시 수정할것
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
	CGameObject* pCursor = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_Shop_Cursor");

	m_iCursorX = dynamic_cast<CUI_Cursor*>(pCursor)->Get_CursorXPos();
	m_iCursorY = dynamic_cast<CUI_Cursor*>(pCursor)->Get_CursorYPos();

	__super::LateUpdate_Object();
}

void CUI_ItemInfo::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3	vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	if (m_bShown &&
		m_tItemInfo.eType != SHOPITEMTYPE::UISHOP_BRANCH_INFO &&
		m_tItemInfo.eType != SHOPITEMTYPE::UISHOP_CLOTH_INFO &&
		m_tItemInfo.eType != SHOPITEMTYPE::UISHOP_LEAF_INFO)
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

		case SHOPITEMTYPE::SHOP_PRICETAG: // Scale, Pos 테스트용임 수정필요함.
			vPos = { m_vDefaultPos.x + 35.f, m_vDefaultPos.y - 60.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
			break;

		case SHOPITEMTYPE::SHOP_VERLINE:
			vPos = { m_vDefaultPos.x + 750.f, m_vDefaultPos.y - 120.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio * 1.5f, fHeight * fRatio * 1.6f, 0.f);
			break;

		case SHOPITEMTYPE::SHOP_HORLINE:
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y - 160.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio * 1.2f, fHeight * fRatio, 0.f);
			break;

		case SHOPITEMTYPE::SHOP_TEXTBOX:
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y - 250.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio * 0.8f, fHeight * fRatio * 1.1f, 0.f);
			break;

		case SHOPITEMTYPE::SHOP_IMGBOX:
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y + 50.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio * 0.8f, fHeight * fRatio * 0.7f, 0.f);
			break;

		case SHOPITEMTYPE::SHOP_WALLET:
			vPos = { m_vDefaultPos.x + 1020.f, m_vDefaultPos.y + 290.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);

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


		// Player Money TextOut //
		_int iMoney;

		CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
		iMoney = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMoney;

		RECT rc = { 1100, 35, WINCX, WINCY / 2 };

		string strMoney = to_string(iMoney);
		wstring sTemp = wstring(strMoney.begin(), strMoney.end());
		LPCWSTR swBuffer = sTemp.c_str();

		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
			swBuffer, lstrlen(swBuffer), &rc, DT_CENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}



	else if (m_bShown &&
			(m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_BRANCH_INFO ||
			m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_CLOTH_INFO ||
			m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_LEAF_INFO))
	{

		if (m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_CLOTH_INFO
			&& m_iCursorX == 0 && m_iCursorY == 0)
		{
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y + 50.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);

			vScale = _vec3(fWidth * fRatio * 1.7f, fHeight * fRatio * 1.7f, 0.f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

			m_pTextureCom->Render_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		if (m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_LEAF_INFO
			&& m_iCursorX == 2 && m_iCursorY == 0)
		{
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y + 50.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);

			vScale = _vec3(fWidth * fRatio * 1.8f, fHeight * fRatio * 1.8f, 0.f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

			m_pTextureCom->Render_Texture(0);
			m_pBufferCom->Render_Buffer();
		}

		if (m_tItemInfo.eType == SHOPITEMTYPE::UISHOP_BRANCH_INFO
			&& m_iCursorX == 1 && m_iCursorY == 0)
		{
			vPos = { m_vDefaultPos.x + 920.f, m_vDefaultPos.y + 50.f, m_vDefaultPos.z };
			m_pTransformCom->Set_Pos(&vPos);

			fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
			fRatio = _float(WINCY) / _float(WINCX);

			vScale = _vec3(fWidth * fRatio * 1.6f, fHeight * fRatio * 1.6f, 0.f);

			m_pTransformCom->Set_Scale(vScale);
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

			m_pTextureCom->Render_Texture(0);
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

	case SHOPITEMTYPE::SHOP_PRICETAG:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_PriceTag"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOP_VERLINE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_VerticalLine"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOP_HORLINE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_HorizontalLine"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOP_TEXTBOX:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_TextBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::UISHOP_BRANCH_INFO:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Branch"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::UISHOP_CLOTH_INFO:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Cloth"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::UISHOP_LEAF_INFO:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Item_Leaf"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOP_IMGBOX:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_TextBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case SHOPITEMTYPE::SHOP_WALLET:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_PlayerWallet"));
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
