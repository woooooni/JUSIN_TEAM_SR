#include "..\..\Header\ImGuiMgr.h"
#include "..\Include\imgui.h"
#include "..\Include\imgui_impl_win32.h"
#include "..\Include\imgui_impl_dx9.h"
#include "Engine_Define.h"
#include "GraphicDev.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CImGuiMgr)
CImGuiMgr::CImGuiMgr()
	: m_pTargetObj(nullptr)
{
}


CImGuiMgr::~CImGuiMgr()
{
	
}

HRESULT CImGuiMgr::Ready_ImGui(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX9_Init(_pDevice);

	return S_OK;
}

void CImGuiMgr::Update_ImGui()
{
	if (m_bEnabled)
	{
		// 매 프레임 호출되어야하는 함수.
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (nullptr != m_pTargetObj)
		{
			DrawObjInfo();
		}

	}

}


void CImGuiMgr::DrawObjInfo()
{
	// 창 생성
	ImGui::Begin("Object Info");

	CTransform* transForm = nullptr;
	transForm = dynamic_cast<CTransform*>(m_pTargetObj->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr == transForm)
		return;

	ImGui::Text("Transform");
	ImGui::Text("Position");

	ImGui::Text("x : ");
	ImGui::SameLine();

	//_vec3& vObjPos;
	//transForm->Get_Info(MATRIX_INFO::INFO_POS, &vObjPos);
	//ImGui::InputFloat("##ObjPosX", &vObjPos.x);

	//ImGui::Text("y : ");
	//ImGui::SameLine();
	//ImGui::InputFloat("##ObjPosY", &pCube->GetPos().y, 0.f, 1.f, "%.3f");

	//ImGui::Text("z : ");
	//ImGui::SameLine();
	//ImGui::InputFloat("##ObjPosZ", &pCube->GetPos().z, 0.f, 1.f, "%.3f");


	//// Rotation
	//ImGui::Text("");
	//ImGui::Text("Rotation");

	//ImGui::Text("yaw :\t");
	//ImGui::SameLine();
	//ImGui::InputFloat("##ObjYaw", &pCube->GetYaw(), 0.f, 1.f, "%.3f");

	//ImGui::Text("pitch :\t");
	//ImGui::SameLine();
	//ImGui::InputFloat("##ObjPitch", &pCube->GetPitch(), 0.f, 1.f, "%.3f");

	//ImGui::Text("roll :\t");
	//ImGui::SameLine();
	//ImGui::InputFloat("##ObjRoll", &pCube->GetRoll(), 0.f, 1.f, "%.3f");

	//// Scale
	//ImGui::Text("");
	//ImGui::Text("Scale");

	//ImGui::Text("x : ");
	//ImGui::SameLine();
	//ImGui::InputText("##x", "", 10, ImGuiInputTextFlags_CharsDecimal);

	//ImGui::Text("y : ");
	//ImGui::SameLine();
	//ImGui::InputText("##y", "", 10, ImGuiInputTextFlags_CharsDecimal);

	//ImGui::Text("z : ");
	//ImGui::SameLine();
	//ImGui::InputText("##x", "", 10, ImGuiInputTextFlags_CharsDecimal);

	ImGui::End();
}

void CImGuiMgr::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
