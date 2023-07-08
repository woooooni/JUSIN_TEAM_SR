#include "ImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "Scene_Tool.h"
#include "BlueBeatle.h"

IMPLEMENT_SINGLETON(CImGuiMgr)
CImGuiMgr::CImGuiMgr()
	: m_bEnabled(true)
	, m_pGraphicDev(nullptr)
	, m_hWnd(nullptr)
	, m_eMode(TOOL_MODE::OBJECT)
	, m_pToolScene(nullptr)
	, m_pTargetObject(nullptr)
	, m_pSelectedObject(nullptr)
	, m_strObjNaming(L"OBJ_TEMP")
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

void CImGuiMgr::Update_ImGui(const _float& fTimeDelta)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Update_Hierachy(fTimeDelta);
	Update_Inspector(fTimeDelta);
	
	// 芒 积己
	ImGui::Begin("Tool");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	if (ImGui::BeginTabItem("Object"))
	{
		m_eMode = TOOL_MODE::OBJECT;
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Terrain"))
	{
		m_eMode = TOOL_MODE::TERRAIN;
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Map"))
	{
		m_eMode = TOOL_MODE::MAP;
		ImGui::EndTabItem();
	}

	switch (m_eMode)
	{
	case TOOL_MODE::OBJECT:
		UpdateObjectTool(fTimeDelta);
		break;
	case TOOL_MODE::TERRAIN:
		UpdateTerrainTool(fTimeDelta);
		break;
	case TOOL_MODE::MAP:
		UpdateMapTool(fTimeDelta);
		break;
	}
	if (ImGui::Button("Save"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{

	}
	ImGui::EndTabBar();
	ImGui::End();
}

void CImGuiMgr::Render_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}



void CImGuiMgr::UpdateObjectTool(const _float& fTimeDelta)
{
	if (m_pTargetObject != nullptr)
	{
		/*CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Terrain");
		if (pTerrain == nullptr)
			return;

		_vec3 vHit;
		if(Engine::IsPicking(pTerrain, &vHit));
			m_pTargetObject->Get_TransformCom()->Set_Info(INFO_POS, &vHit);*/
	}
	else
	{
		if (m_pSelectedObject != nullptr)
		{
			m_pSelectedObject->Update_Object(0.f);
			m_pSelectedObject->LateUpdate_Object();
			m_pSelectedObject->Render_Object();
			CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Terrain");
			if (pTerrain == nullptr)
				return;

			_vec3 vHit;
			if (Engine::IsPicking(pTerrain, &vHit))
			{
				m_pSelectedObject->Get_TransformCom()->Set_Info(INFO_POS, &vHit);
				if (KEY_TAP(KEY::LBTN))
					CreateObj(m_eSelectedObjType, vHit);

			}
				
		}
	}

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;

	ImGui::BeginTabBar("Tab", tab_bar_flags);
	if (ImGui::BeginTabItem("Environment"))
	{
		if (ImGui::Button("Blue_Beatle"))
		{
			m_pTargetObject = nullptr;
			m_eSelectedObjType = OBJ_SELECTED::BLUE_BEATLE;
			if (nullptr != m_pSelectedObject)
			{
				Safe_Release(m_pSelectedObject);
				m_pSelectedObject = nullptr;
			}
			m_pSelectedObject = CBlueBeatle::Create(m_pGraphicDev);
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Monster"))
	{
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Interaction"))
	{
		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();
}

void CImGuiMgr::UpdateTerrainTool(const _float& fTimeDelta)
{
	CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Terrain");
	if (nullptr == pTerrain)
		return;
	
}

void CImGuiMgr::UpdateMapTool(const _float& fTimeDelta)
{

}

void CImGuiMgr::CreateObj(OBJ_SELECTED _eSelected, _vec3& vHit)
{
	switch (_eSelected)
	{
	case OBJ_SELECTED::BLUE_BEATLE :
		CGameObject* pCloneObj = CBlueBeatle::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Info(INFO_POS, &vHit);
		Engine::Get_Layer(LAYER_TYPE::BACK_GROUND)->Add_GameObject(m_strObjNaming, pCloneObj);
		break;
	}
}

void CImGuiMgr::DeleteObj()
{
}

void CImGuiMgr::Update_Inspector(const _float& fTimeDelta)
{
	ImGui::Begin("Inspector");
	if (m_pTargetObject != nullptr)
	{
		CTransform* pTargetTransform = m_pTargetObject->Get_TransformCom();
		if (pTargetTransform != nullptr)
		{
			_vec3 vPos, vScale;
			pTargetTransform->Get_Info(INFO_POS, &vPos);
			vScale = pTargetTransform->Get_Scale();

			ImGui::Text("Name : ");
			ImGui::SameLine();

			string strName = std::string().assign(m_pTargetObject->Get_Name().begin(), m_pTargetObject->Get_Name().end());
			ImGui::Text(strName.c_str());

			ImGui::Text("");
			ImGui::Text("");

			ImGui::Text("Transform");

			// Position
			ImGui::Text("");
			ImGui::Text("Position");
			ImGui::Text("x : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ObjPosX", &vPos.x, 0.f, 1.f, "%.3f");

			ImGui::Text("y : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ObjPosY", &vPos.y, 0.f, 1.f, "%.3f");

			ImGui::Text("z : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ObjPosZ", &vPos.z, 0.f, 1.f, "%.3f");

			// Scale
			ImGui::Text("");
			ImGui::Text("Scale");

			ImGui::Text("x : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ScaleX", &vScale.x, 0.f, 0.5f, "%.3f");

			ImGui::Text("y : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ScaleY", &vScale.y, 0.f, 0.5f, "%.3f");

			ImGui::Text("z : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ScaleZ", &vScale.z, 0.f, 0.5f, "%.3f");

			pTargetTransform->Set_Info(INFO_POS, &vPos);
			pTargetTransform->Set_Scale(vScale);
		}
	}
	ImGui::End();
}

void CImGuiMgr::Update_Hierachy(const _float& fTimeDelta)
{
	// 芒 积己
	ImGui::Begin("Hierachy");
	if (nullptr != m_pToolScene)
	{
		ImGui::BeginListBox(" ", ImVec2(400, 350));
		for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
		{
			if (nullptr == m_pToolScene->Get_Layer((LAYER_TYPE)i))
				continue;

			const vector<CGameObject*>& vecObjects = m_pToolScene->Get_Layer((LAYER_TYPE)i)->Get_GameObjectVec();

			for (auto iter : vecObjects)
			{
				string strName = std::string().assign(iter->Get_Name().begin(), iter->Get_Name().end());
				if (ImGui::Selectable(strName.c_str(), iter == m_pTargetObject))
				{
					m_pTargetObject = iter;
				}
			}
		}
		ImGui::EndListBox();
	}
	ImGui::End();
}

void CImGuiMgr::Free()
{
	Safe_Release(m_pSelectedObject);
	Safe_Release(m_pTargetObject);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
