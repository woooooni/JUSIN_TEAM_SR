#include "ImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "Scene_Tool.h"
#include "GameObject.h"
#include "BlueBeatle.h"
#include "DesertRhino.h"
#include "TrashBig.h"
#include "Tile.h"
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
		if (m_pSelectedObject)
		{
			Safe_Release(m_pSelectedObject);
			m_pSelectedObject = nullptr;
		}

		if (m_pTargetObject)
			m_pTargetObject = nullptr;
			

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

	ImGui::Text("");
	ImGui::Text("");

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
		if (KEY_TAP(KEY::DEL))
			DeleteObj();
	}
	else
	{
		if (m_pSelectedObject != nullptr)
		{
			m_pSelectedObject->Update_Object(fTimeDelta);
			m_pSelectedObject->LateUpdate_Object();
			m_pSelectedObject->Render_Object();
			CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Terrain");

			if (pTerrain == nullptr)
				return;

			_vec3 vHit, vPos;
			if (Engine::IsPicking(pTerrain, &vHit))
			{
				m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

				vHit.y = vPos.y;
				m_pSelectedObject->Get_TransformCom()->Set_Info(INFO_POS, &vHit);
				if (KEY_TAP(KEY::LBTN))
					CreateObj(m_eSelectedObjType, vHit);

				if (KEY_TAP(KEY::RBTN))
				{
					Safe_Release(m_pSelectedObject);
					m_pSelectedObject = nullptr;
				}
			}
		}
	}

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	if (ImGui::BeginTabItem("Monster"))
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

		if (ImGui::Button("Desert_Rino"))
		{
			m_pTargetObject = nullptr;
			m_eSelectedObjType = OBJ_SELECTED::DESERT_RHINO;
			if (nullptr != m_pSelectedObject)
			{
				Safe_Release(m_pSelectedObject);
				m_pSelectedObject = nullptr;
			}
			m_pSelectedObject = CDesertRhino::Create(m_pGraphicDev);
		}

		if (ImGui::Button("Trash_Big"))
		{
			m_pTargetObject = nullptr;
			m_eSelectedObjType = OBJ_SELECTED::TRASH_BIG;
			if (nullptr != m_pSelectedObject)
			{
				Safe_Release(m_pSelectedObject);
				m_pSelectedObject = nullptr;
			}
			m_pSelectedObject = CTrashBig::Create(m_pGraphicDev);
		}

		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Tile"))
	{
		CTexture* pTileTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Tile"));
		if (pTileTex != nullptr)
		{
			for (size_t i = 0; i < pTileTex->Get_Size(); ++i)
			{
				if (i % 4 != 0)
					ImGui::SameLine();

				if (ImGui::ImageButton(pTileTex->Get_TextureVec()[i], ImVec2(50.f, 50.f)))
				{
					if (m_pSelectedObject)
					{
						Safe_Release(m_pSelectedObject);
						m_pSelectedObject = nullptr;
					}
					m_pTargetObject = nullptr;
					m_pSelectedObject = CTile::Create(m_pGraphicDev);
					m_pSelectedObject->Get_TextureCom()->Set_Idx(i);
					m_eSelectedObjType = OBJ_SELECTED::TILE;
				}
			}
		}
			
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

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	
	if (ImGui::BeginTabItem("Terrain"))
	{
		const vector<LPDIRECT3DBASETEXTURE9>& vecTexture = pTerrain->Get_TextureCom()->Get_TextureVec();
		for (_uint i = 0; i < vecTexture.size(); ++i)
		{
			if (i > 0)
				ImGui::SameLine();

			if (ImGui::ImageButton(vecTexture[i], ImVec2(50.f, 50.f)))
				pTerrain->Get_TextureCom()->Set_Idx(i);
		}
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

void CImGuiMgr::UpdateMapTool(const _float& fTimeDelta)
{

}

void CImGuiMgr::UpdateTileTool(const _float& fTimeDelta)
{
}

void CImGuiMgr::CreateObj(OBJ_SELECTED _eSelected, _vec3& vHit)
{
	CGameObject* pCloneObj = nullptr;
	_vec3 vPos;
	switch (_eSelected)
	{
	case OBJ_SELECTED::BLUE_BEATLE :
		pCloneObj = CBlueBeatle::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming, pCloneObj);
		break;

	case OBJ_SELECTED::DESERT_RHINO:
		pCloneObj = CDesertRhino::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming, pCloneObj);
		break;

	case OBJ_SELECTED::TRASH_BIG:
		pCloneObj = CTrashBig::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming, pCloneObj);
		break;

	case OBJ_SELECTED::TILE:
		pCloneObj = CTile::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(m_strObjNaming, pCloneObj);
		break;
	}

	pCloneObj->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos = _vec3(vHit.x, vPos.y, vHit.z);
	pCloneObj->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
}

void CImGuiMgr::DeleteObj()
{
	if (nullptr == m_pTargetObject)
		return;

	for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
	{
		CLayer* pLayer = m_pToolScene->Get_Layer((LAYER_TYPE)i);
		vector<CGameObject*>& vecObj = pLayer->Get_GameObjectVec();

		auto iter = vecObj.begin();
		for (; iter != vecObj.end(); ++iter)
		{
			if ((*iter) == m_pTargetObject)
			{
				Safe_Release(m_pTargetObject);
				vecObj.erase(iter);
				m_pTargetObject = nullptr;
				return;
			}
		}
	}
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

			_uint j = 0;
			for (auto& iter : vecObjects)
			{
				string strName = std::string().assign(iter->Get_Name().begin(), iter->Get_Name().end());
				if (ImGui::Selectable((strName + to_string(j)).c_str(), iter == m_pTargetObject))
				{
					m_pTargetObject = iter;

					CCamera* pCamera = m_pToolScene->Get_MainCamera();

					_vec3 vTargetPos, vCameraPos;
					iter->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
					pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCameraPos);
					
					vCameraPos.x = vTargetPos.x;
					vCameraPos.y = vTargetPos.y + 5.f;
					vCameraPos.z = vTargetPos.z - 10.f;

					_vec3 vDir = vTargetPos - vCameraPos;
					D3DXVec3Normalize(&vDir, &vDir);

					_vec3 vRight;
					D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vDir);

					_vec3 vUp;
					D3DXVec3Cross(&vUp, &vDir, &vRight);


					pCamera->Get_TransformCom()->Set_Info(INFO_LOOK, &vDir);
					pCamera->Get_TransformCom()->Set_Info(INFO_RIGHT, &vRight);
					pCamera->Get_TransformCom()->Set_Info(INFO_UP, &vUp);
					pCamera->Get_TransformCom()->Set_Info(INFO_POS, &vCameraPos);
				}

				j++;
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
