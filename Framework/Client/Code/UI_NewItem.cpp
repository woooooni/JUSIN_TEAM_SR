#include "UI_NewItem.h"
#include "Export_Function.h"

CUI_NewItem::CUI_NewItem(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_NewItem::CUI_NewItem(const CUI_NewItem& rhs) : CUI(rhs)
{
}

CUI_NewItem::~CUI_NewItem()
{
}

HRESULT CUI_NewItem::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_tInfo.fX = WINCX / 2;
	m_tInfo.fY = WINCY / 2;

	m_pWindow = CUI_Notification::Create(m_pGraphicDev, 0.8f, 0.79f);
	if (m_pWindow != nullptr)
		m_pWindow->Set_Owner(this);

	m_pFrame = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_SLOT);
	if (m_pFrame != nullptr)
		m_pFrame->Set_Owner(this);

	m_pCloseKey = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CLOSE);
	if (m_pCloseKey != nullptr)
		m_pCloseKey->Set_Owner(this);

	return S_OK;
}

_int CUI_NewItem::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	m_pWindow->Update_Object(fTimeDelta);

	m_pCloseKey->Update_Object(fTimeDelta);
	m_pFrame->Update_Object(fTimeDelta);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_NewItem::LateUpdate_Object(void)
{
	m_pWindow->LateUpdate_Object();

	m_pCloseKey->LateUpdate_Object();
	m_pFrame->LateUpdate_Object();

	m_fCurWidth = m_pWindow->Get_CurWidth();
	m_fMaxWidth = m_pWindow->Get_MaxWidth();

	if (m_fCurWidth > m_fMaxWidth)
	{
		m_pCloseKey->Set_Shown(true);
		m_pFrame->Set_Shown(true);

		m_bShown = true;
	}

	__super::LateUpdate_Object();
}

void CUI_NewItem::Render_Object(void)
{

	if (m_bShown)
	{
		// Rect 범위 확인용
		
		// 아이템명
		RECT rcTitle = { 0, 100, WINCX, WINCY / 2 };
		TCHAR szTitleBuf[256] = L"";

		swprintf_s(szTitleBuf, L"장사풍뎅이의 힘");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szTitleBuf, lstrlen(szTitleBuf), &rcTitle, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// 아이템설명
		RECT rcContents = { 0, 290, WINCX, WINCY };
		TCHAR szConBuf[256] = L"";

		swprintf_s(szConBuf, L"장사 풍뎅이의 시험을 통해 받은 힘.");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szConBuf, lstrlen(szConBuf), &rcContents, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CUI_NewItem::Key_Input()
{
	if (m_bShown)
		if (KEY_TAP(KEY::Z))
			Set_Active(false);
}

void CUI_NewItem::Get_ItemInfo(ITEM_CODE _eCodeType)
{
	m_pTextureCom->Set_Idx((_uint) _eCodeType);
	Get_Layer(LAYER_TYPE::UI)->Add_GameObject(L"ItemInfo", this);
}

CUI_NewItem* CUI_NewItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NewItem* pInstance = new CUI_NewItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NewItem Window Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NewItem::Free()
{
}
