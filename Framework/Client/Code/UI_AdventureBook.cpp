#include "../Include/stdafx.h"
#include "UI_AdventureBook.h"
#include "UI_Pages.h"
#include "UI_BookBackground.h"

CUI_AdventureBook::CUI_AdventureBook(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIPages(nullptr)
	, m_pUIBackGround(nullptr)
{
}

CUI_AdventureBook::CUI_AdventureBook(const CUI_AdventureBook& rhs)
	: CUI(rhs)
	, m_pUIPages(rhs.m_pUIPages)
	, m_pUIBackGround(rhs.m_pUIBackGround)
{

}

CUI_AdventureBook::~CUI_AdventureBook()
{

}


HRESULT CUI_AdventureBook::Ready_Object(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pUIPages = CUI_Pages::Create(pGraphicDev);
	m_pUIBackGround = CUI_BookBackground::Create(pGraphicDev);

	NULL_CHECK_RETURN(m_pUIPages, E_FAIL);
	NULL_CHECK_RETURN(m_pUIBackGround, E_FAIL);

	return S_OK;
}

_int CUI_AdventureBook::Update_Object(const _float& fTimeDelta)
{
	Debug_Input();
	m_pUIBackGround->Update_Object(fTimeDelta);
	m_pUIPages->Update_Object(fTimeDelta);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_AdventureBook::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_AdventureBook::Render_Object(void)
{
	__super::Render_Object();
}

CUI_AdventureBook* CUI_AdventureBook::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_AdventureBook* pInstance = new CUI_AdventureBook(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pGraphicDev)))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI_AdventureBook Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_AdventureBook::Free()
{
	Safe_Release(m_pUIBackGround);
	Safe_Release(m_pUIPages);
	__super::Free();
}
