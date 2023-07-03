#include "../Include/stdafx.h"
#include "ImGuiMgr.h"

CImGuiMgr::CImGuiMgr()
	: m_bEnabled(true)
	, m_pGraphicDev(nullptr)
{

}

CImGuiMgr::~CImGuiMgr()
{

}

HRESULT CImGuiMgr::Ready_ImGuiMgr(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice)
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);
	return S_OK;
}

void CImGuiMgr::Update_ImGui()
{
}

void CImGuiMgr::Render_ImGui()
{
}

void CImGuiMgr::Free()
{

}
