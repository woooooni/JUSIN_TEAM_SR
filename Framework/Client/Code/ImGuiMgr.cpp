#include "ImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CImGuiMgr)
CImGuiMgr::CImGuiMgr()
	: m_bEnabled(true)
	, m_pGraphicDev(nullptr)
	, m_hWnd(nullptr)
	, m_eMode(TOOL_MODE::OBJECT)
	, m_pToolScene(nullptr)
	, m_pTargetObject(nullptr)
{

}

CImGuiMgr::~CImGuiMgr()
{
	Free();
}

HRESULT CImGuiMgr::Ready_ImGuiMgr(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice)
{

	m_hWnd = _hWnd;
	m_pGraphicDev = _pDevice;
	_pDevice->AddRef();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	

	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);
	

	return S_OK;
}

void CImGuiMgr::Update_ImGui()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	switch (m_eMode)
	{
	case TOOL_MODE::OBJECT:
		UpdateObjectTool();
		break;
	case TOOL_MODE::TERRAIN:
		UpdateTerrainTool();
		break;
		UpdateMapTool();
	case TOOL_MODE::MAP:
		break;
	}


}

void CImGuiMgr::Render_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CImGuiMgr::UpdateObjectTool()
{
	// Ã¢ »ý¼º
	ImGui::Begin("Object_Tool");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	if (ImGui::BeginTabBar("MyTabBar1", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Info"))
		{
			if (m_pTargetObject != nullptr)
			{
				CTransform* pTargetTransform = m_pTargetObject->Get_TransformCom();
				if (pTargetTransform != nullptr)
				{
					_vec3 vPos;
					pTargetTransform->Get_Info(INFO_POS, &vPos);

					ImGui::Text("Transform");
					ImGui::Text("x : ");
					ImGui::SameLine();
					ImGui::InputFloat("##ObjPosX", &vPos.x, 0.f, 1.f, "%.3f");

					ImGui::Text("y : ");
					ImGui::SameLine();
					ImGui::InputFloat("##ObjPosY", &vPos.y, 0.f, 1.f, "%.3f");

					ImGui::Text("z : ");
					ImGui::SameLine();
					ImGui::InputFloat("##ObjPosZ", &vPos.z, 0.f, 1.f, "%.3f");

					pTargetTransform->Set_Info(INFO_POS, &vPos);
				}
			}
			

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void CImGuiMgr::UpdateTerrainTool()
{

}

void CImGuiMgr::UpdateMapTool()
{

}

void CImGuiMgr::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
