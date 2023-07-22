#include "UI_NewQuest.h"
#include "Export_Function.h"

CUI_NewQuest::CUI_NewQuest(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_NewQuest::CUI_NewQuest(const CUI_NewQuest& rhs) : CUI(rhs)
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

//	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
//	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
//	_float fRatio = _float(WINCY) / _float(WINCX);
//
//	m_fMaxWidth = fWidth * fRatio * 2.1f;
//	m_fMaxHeight = fHeight * fRatio * 2.1f * 0.79f;
//
//	if (m_fCurWidth < m_fMaxWidth)
//		m_fCurWidth += m_fCurWidth * fRatio * fTimeDelta * m_fSpeed;
//
//	if (m_fCurHeight < m_fMaxHeight)
//		m_fCurHeight += m_fCurHeight * fRatio * fTimeDelta * m_fSpeed;

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

//	if ((m_fCurHeight == m_fMaxHeight) && (m_fCurWidth == m_fMaxWidth))
//		m_bShown = true;

//	m_fCurWidth = m_pWindow->Get_UI_Info().fCX;
	m_fCurWidth = m_pWindow->Get_CurWidth();
	m_fMaxWidth = m_pWindow->Get_MaxWidth();

//	if (m_fCurHeight > m_fMaxHeight)
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
	//CUI::Render_Object();
//	if (m_bShown)
//	{
//		_matrix matPreView, matPreProj;
//
//		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
//		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
//
//		_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) ,
//					((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
//
//		m_pTransformCom->Set_Pos(&vPos);
//
//		_float fRatio = _float(WINCY) / _float(WINCX);
//
//		_vec3 vScale = _vec3(m_fCurWidth, m_fCurHeight, 0.f);
//		m_pTransformCom->Set_Scale(vScale);
//		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
//
//		m_pTextureCom->Render_Texture(0);
//		m_pBufferCom->Render_Buffer();
//	}

	if (m_bShown)
	{
		// 퀘스트 창 Title
		RECT rcTitle = { 0, 0, WINCX, WINCY / 2 - 50.f };
		TCHAR szTitleBuf[256] = L"";

		swprintf_s(szTitleBuf, L"퀘스트를 받았다!");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szTitleBuf, lstrlen(szTitleBuf), &rcTitle, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// 퀘스트 명
		RECT rcQuest = { WINCX / 4 + 10, 0, WINCX, WINCY - 90 };
		TCHAR szQuestBuf[256] = L"";

		swprintf_s(szQuestBuf, L"태양의 마을 찾기");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szQuestBuf, lstrlen(szQuestBuf), &rcQuest, DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// 퀘스트 내용
		RECT rcContents = { 0, 120, WINCX, WINCY };
		TCHAR szConBuf[256] = L"";

		swprintf_s(szConBuf, L"알 수 없는 목소리가 동쪽의 해가 뜨는 마을로 가라고 한다.\n동쪽... 동쪽은 오른쪽이라고 배웠다.");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szConBuf, lstrlen(szConBuf), &rcContents, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUI_NewQuest::Key_Input()
{
	if (m_bShown)
	{
		if (KEY_TAP(KEY::Z))
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


