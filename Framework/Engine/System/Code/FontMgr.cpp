
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

HRESULT CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pDevice)
{
	m_vecFont.reserve(10);

	/////////////////
	// �Ű�����
	// �׷���ī�� �������̽�, ���� ������, ���� ������, ����, 1,
	// ����, DEFAULT_CHARSET, ���е�, DEFAULT_QUALITY, 0, �۲�, LPD3DXFONT ������

/////////////////
	LPD3DXFONT	pFont = nullptr;
	if (FAILED(D3DXCreateFont(pDevice, 30, 30, 100, 1,
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"ī��24 ����� ����", &pFont)))
	{
		MSG_BOX("Create Font Failed");
		return E_FAIL;
	}
	m_vecFont.push_back(pFont);

	if (FAILED(D3DXCreateFont(pDevice, 30, 30, 100, 1,
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"ī��24 �����", &pFont)))
	{
		MSG_BOX("Create Font Failed");
		return E_FAIL;
	}
	m_vecFont.push_back(pFont);

	if (FAILED(D3DXCreateFont(pDevice, 25, 25, 100, 1,
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"ī��24 �����", &pFont)))
	{
		MSG_BOX("Create Font Failed");
		return E_FAIL;
	}
	m_vecFont.push_back(pFont);

	return S_OK;
}



void CFontMgr::Free(void)
{
	for(size_t i = 0; i<m_vecFont.size(); ++i)
		Safe_Release(m_vecFont[i]);

}
