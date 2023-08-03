#include "UI_Shop.h"
#include "Export_Function.h"
#include "UI_ItemInfo.h"
#include "GameMgr.h"

CUI_Shop::CUI_Shop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
	m_vecShown.resize(3);

	for (auto& iter : m_vecShown)
	{
		iter = true;
	}
}

CUI_Shop::CUI_Shop(const CUI_Shop& rhs)
	: CUI(rhs)
{
}
CUI_Shop::~CUI_Shop()
{
}
HRESULT CUI_Shop::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	m_tInfo.fX = WINCX / 2 - 180.f;
	m_tInfo.fY = WINCY / 2 + 80.f;
	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	m_vecShopIcon.reserve(SHOPITEMTYPE::SHOPITEM_END);

	for (_uint i = 0; i < SHOPITEMTYPE::SHOPITEM_END; i++)
	{
		m_vecShopIcon.push_back(CUI_ItemInfo::Create(m_pGraphicDev, (SHOPITEMTYPE)i));
	}

	m_pCursor = CUI_Cursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCursor, E_FAIL);
	return S_OK;
}
_int CUI_Shop::Update_Object(const _float& fTimeDelta)
{
	Key_Input();

	if (m_bShown)
	{
		m_pCursor->Update_Object(fTimeDelta);

		m_iCursorX = Get_Cursor()->Get_CursorXPos();
		m_iCursorY = Get_Cursor()->Get_CursorYPos();

		for (auto& iter : m_vecShopIcon)
			iter->Update_Object(fTimeDelta);
		__super::Update_Object(fTimeDelta);
	}

	CPlayer* player = dynamic_cast<CPlayer*>(Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player"));
	NULL_CHECK_RETURN(player, E_FAIL);

	return S_OK;
}
void CUI_Shop::LateUpdate_Object(void)
{
	m_pCursor->LateUpdate_Object();

	if (m_iCursorX > 2 || m_iCursorY > 0)
	{
		m_strItemInfo = L"";
		m_strItemName = L"";
		m_iItemPrice = 0;
	}

	if (m_iCursorY == 0)
	{
		switch (m_iCursorX)
		{
		case 0: // LIGHT
			m_strItemInfo = L"주변을 밝게\n밝혀주는 모자";
			break;

		case 1: // MASK
			m_strItemInfo = L"번개를 불러일으키는\n무시무시한 모자";
			break;

		case 2: // MISSILE
			m_strItemInfo = L"미래적인 공격을\n퍼부울 수 있는\n강력한 모자";
			break;

		default:
			break;
		}
	}

	for (auto& iter : m_vecShopIcon)
		iter->LateUpdate_Object();
	__super::LateUpdate_Object();
}
void CUI_Shop::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3 vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.1f };
	m_pTransformCom->Set_Pos(&vPos);
	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(m_tInfo.fCX * fRatio * 2.f, m_tInfo.fCY * fRatio * 2.f, 0.f);
	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	for (auto& iter : m_vecShopIcon)
	{
		iter->Render_Object();
	}

	if (m_iCursorX < 3 && m_iCursorX >= 0)
	{
		if (m_vecShown[m_iCursorX])
		{
			// 아이템명
			RECT rcTitle = { WINCX * 3 / 4 - 30, WINCY / 3 + 40, WINCX, WINCY * 2 / 3 };
			Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
				m_strItemName.c_str(), m_strItemName.length(), &rcTitle,
				DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 128, 128, 128));

			// 아이템설명
			RECT rcContents = { WINCX * 3 / 4 - 30, WINCY / 2 + 70, WINCX, WINCY };
			Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD_SMALL)->DrawText(NULL,
				m_strItemInfo.c_str(), m_strItemInfo.length(), &rcContents,
				DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 140, 140, 140));

			// 아이템가격
			if (m_iItemPrice != 0)
			{
				string strMoney = to_string(m_iItemPrice);
				wstring sTemp = wstring(strMoney.begin(), strMoney.end());
				LPCWSTR swBuffer = sTemp.c_str();

				RECT rcPrice = { WINCX - 130, WINCY / 3 + 60, WINCX, WINCY * 2 / 3 };
				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD_SMALL)->DrawText(NULL,
					swBuffer, lstrlen(swBuffer), &rcPrice,
					DT_NOCLIP, D3DCOLOR_ARGB(255, 90, 90, 90));
			}
		}
	}

	__super::Render_Object();
}

void CUI_Shop::Remove_Item(SHOPITEMTYPE pType)
{
	dynamic_cast<CUI_ItemInfo*>(m_vecShopIcon[(_uint)pType])->Set_Item(nullptr);
	dynamic_cast<CUI_ItemInfo*>(m_vecShopIcon[(_uint)pType + 1])->Set_Item(nullptr);
}

void CUI_Shop::Set_ItemInfo(ITEM_CODE _eCode, _uint _iPrice)
{
	//m_strItemInfo = CItem::Get_Explain(_eCode); // 줄바꿈
	m_strItemName = CItem::Get_Name(_eCode);
	m_iItemPrice = _iPrice;
}

HRESULT CUI_Shop::Ready_Component()
{
	CComponent* pComponent = nullptr;
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Background"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	return S_OK;
}
void CUI_Shop::Key_Input()
{
	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
		{
			Stop_Sound(CHANNELID::SOUND_UI);
			Play_Sound(L"SFX_67_UIBig_Open.wav", CHANNELID::SOUND_UI, 0.5f);

			m_bShown = true;
			m_pCursor->Set_Shown(true);
			for (auto& iter : m_vecShopIcon)
				dynamic_cast<CUI_ItemInfo*>(iter)->Set_Shown(true);
		}
		else
		{
			Stop_Sound(CHANNELID::SOUND_UI);
			Play_Sound(L"SFX_68_UIBig_Close.wav", CHANNELID::SOUND_UI, 0.5f);

			m_bShown = false;
			m_pCursor->Set_Shown(false);
			for (auto& iter : m_vecShopIcon)
				dynamic_cast<CUI_ItemInfo*>(iter)->Set_Shown(false);
		}
	}

	if (m_bShown)
	{
		CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();;

		if (pPlayer != nullptr)
		{
			if (KEY_TAP(KEY::ENTER) && m_iCursorX >= 0 && m_iCursorX < 3 && m_iCursorY == 0
				&& m_vecShown[m_iCursorX]
				&& m_iItemPrice <= dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMoney)
			{
				m_vecShown[m_iCursorX] = false;

				Stop_Sound(CHANNELID::SOUND_UI);
				Play_Sound(L"SFX_201_ShopBuy.wav", CHANNELID::SOUND_UI, 0.5f);
			}
		}
	}
}

CUI_Shop* CUI_Shop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Shop* pInstance = new CUI_Shop(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Shop Create Failed");
		return nullptr;
	}
	return pInstance;
}
void CUI_Shop::Free()
{
}