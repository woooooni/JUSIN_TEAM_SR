#include "UI_NewQuest.h"
#include "Export_Function.h"

CUI_NewQuest::CUI_NewQuest(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CUI(pGraphicDev)
{
}

CUI_NewQuest::CUI_NewQuest(const CUI_NewQuest& rhs) 
	: CUI(rhs)
{
}

CUI_NewQuest::~CUI_NewQuest()
{
}

HRESULT CUI_NewQuest::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_pWindow = CUI_Notification::Create(m_pGraphicDev, 1.f, 0.79f);
	if (m_pWindow != nullptr)
		m_pWindow->Set_Owner(this);
	
	m_pTitleBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_TITLE);
	if (m_pTitleBox != nullptr)
		m_pTitleBox->Set_Owner(this);

	m_pContentsBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CONTENTS);
	if (m_pContentsBox != nullptr)
		m_pContentsBox->Set_Owner(this);

	m_pExclamIcon = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_ICON);
	if (m_pExclamIcon != nullptr)
		m_pExclamIcon->Set_Owner(this);

	m_pCloseKey = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CLOSE);
	if (m_pCloseKey != nullptr)
		m_pCloseKey->Set_Owner(this);

	return S_OK;
}

_int CUI_NewQuest::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	m_pWindow->Update_Object(fTimeDelta);

	m_pCloseKey->Update_Object(fTimeDelta);
	m_pTitleBox->Update_Object(fTimeDelta);
	m_pContentsBox->Update_Object(fTimeDelta);
	m_pExclamIcon->Update_Object(fTimeDelta);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_NewQuest::LateUpdate_Object(void)
{
	m_pWindow->LateUpdate_Object();

	m_pCloseKey->LateUpdate_Object();
	m_pTitleBox->LateUpdate_Object();
	m_pContentsBox->LateUpdate_Object();
	m_pExclamIcon->LateUpdate_Object();

	m_fCurWidth = m_pWindow->Get_CurWidth();
	m_fMaxWidth = m_pWindow->Get_MaxWidth();

	if (m_fCurWidth > m_fMaxWidth)
	{
		m_pCloseKey->Set_Shown(true);
		m_pTitleBox->Set_Shown(true);
		m_pContentsBox->Set_Shown(true);
		m_pExclamIcon->Set_Shown(true);

		m_bShown = true;
	}

	__super::LateUpdate_Object();
}

void CUI_NewQuest::Render_Object(void)
{

	if (m_bShown)
	{
		// Äù½ºÆ® Ã¢ Title
		RECT rcTitle = { 0, 0, WINCX, WINCY / 2 - 50.f };
		TCHAR szTitleBuf[256] = L"";

		swprintf_s(szTitleBuf, L"Äù½ºÆ®¸¦ ¹Þ¾Ò´Ù!");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szTitleBuf, lstrlen(szTitleBuf), &rcTitle, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// Äù½ºÆ® ¸í
		RECT rcQuest = { WINCX / 4 + 30, 5, WINCX, WINCY - 100 };
		TCHAR szQuestBuf[256] = L"";

		swprintf_s(szQuestBuf, m_strTitle.c_str());
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szQuestBuf, lstrlen(szQuestBuf), &rcQuest, DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// Äù½ºÆ® ³»¿ë
		RECT rcContents = { 0, 170, WINCX, WINCY };
		TCHAR szConBuf[256] = L"";

		swprintf_s(szConBuf, m_strDesc.c_str());
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szConBuf, lstrlen(szConBuf), &rcContents, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUI_NewQuest::Key_Input()
{
	if (m_bShown)
	{
		if (KEY_AWAY(KEY::TAB))
		{
			Set_Active(false);

//			m_bShown = false;
//
//			m_pCloseKey->Set_Shown(false);
//			m_pTitleBox->Set_Shown(false);
//			m_pContentsBox->Set_Shown(false);
//			m_pExclamIcon->Set_Shown(false);
		}
	}
}

CUI_NewQuest* CUI_NewQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NewQuest* pInstance = new CUI_NewQuest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("New Quest Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NewQuest::Free()
{
}


