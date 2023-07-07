#include "../Include/stdafx.h"
#include "UI_AdventureBook.h"

CUI_AdventureBook::CUI_AdventureBook(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIPages(nullptr)
{
}

CUI_AdventureBook::CUI_AdventureBook(const CUI_AdventureBook& rhs)
	: CUI(rhs)
	, m_pUIPages(rhs.m_pUIPages)
{

}

CUI_AdventureBook::~CUI_AdventureBook()
{

}

CUIPages* CUI_AdventureBook::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CUI_AdventureBook::Free()
{
}
