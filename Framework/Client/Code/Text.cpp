#include "Text.h"
#include "Export_Function.h"

CTextBox::CTextBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CTextBox::CTextBox(const CTextBox& rhs)
	: CUI(rhs)
{
}

CTextBox::~CTextBox()
{
}
HRESULT CTextBox::Ready_Object(void)
{
	return S_OK;
}

_int CTextBox::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CTextBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTextBox::Render_Object(void)
{
	RECT rc = { 0, WINCY / 2, WINCX, WINCY };
	//GetClientRect(g_hWnd, &rc);

	TCHAR szBuf[256] = L"";
	swprintf_s(szBuf, L"Hello Ogu World!");
	CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
		szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(100, 255, 255, 255));
	// NULL, ����� Text, Text����(Ȥ�� -1 -> �ڵ����� ���ڿ� ���� ���),
	// �۲� ��� �簢 ����, ��� ����, ����
}

CTextBox* CTextBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTextBox* pInstance = new CTextBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Text Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTextBox::Free()
{
}
