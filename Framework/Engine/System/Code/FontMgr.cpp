
#include "FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr(void)
{
}

CFontMgr::~CFontMgr(void)
{
	Free();
}

HRESULT CFontMgr::Ready_Font(HINSTANCE hInst, HWND hWnd)
{
	m_vecFont.reserve(10);

	LPD3DXFONT	pFont = nullptr;
	D3DXCreateFont()
	D3DXCreateFont

	return E_NOTIMPL;
}

void CFontMgr::Free(void)
{
}
