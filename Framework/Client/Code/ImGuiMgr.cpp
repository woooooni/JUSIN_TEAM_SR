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
	, m_strObjNaming(L"Obj_Name_")
	, m_vObjScale(_vec3(1.f, 1.f, 1.f))
	, m_iObjNum(0)
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
	

	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\batang.ttc", 12.f, NULL, io.Fonts->GetGlyphRangesKorean());

	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);
	

	return S_OK;
}

void CImGuiMgr::Update_ImGui(const _float& fTimeDelta)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Input(fTimeDelta);

	Update_Help(fTimeDelta);
	Update_Hierachy(fTimeDelta);
	Update_Inspector(fTimeDelta);
	
	// â ����
	ImGui::Begin("Tool");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	ImGui::Text("");
	
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

	ImGui::Text("");
	ImGui::Text("");

	if (ImGui::Button("Save"))
	{
		m_pToolScene->Save_Data(L"../Bin/Data");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		m_pToolScene->Load_Data(L"../Bin/Data");
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
			m_pSelectedObject->Get_TransformCom()->Set_Scale(m_vObjScale);
			CBoxCollider* pBoxCol = dynamic_cast<CBoxCollider*>(m_pSelectedObject->Get_ColliderCom());

			if (pBoxCol != nullptr)
				pBoxCol->Set_Scale(m_vObjScale);

			CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::TERRAIN)->Find_GameObject(L"Terrain");

			if (pTerrain == nullptr)
				return;

			_vec3 vHit, vPos;
			if (Engine::IsPicking(pTerrain, &vHit))
			{
				m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

				if (m_eSelectedObjType == OBJ_SELECTED::TILE)
				{
					vHit.x = _int(vHit.x) + 0.5f;
					vHit.y = _int(vHit.y) + 0.5f;
					vHit.z = _int(vHit.z) + 0.5f;
				}
					

				vHit.y = vPos.y;
				m_pSelectedObject->Get_TransformCom()->Set_Info(INFO_POS, &vHit);
				if (KEY_TAP(KEY::LBTN))
				{
					CreateObj(m_eSelectedObjType, vHit);
				}

				if (KEY_TAP(KEY::RBTN))
				{
					ResetSelectTarget();
					return;
				}
			}

			m_pSelectedObject->Update_Object(fTimeDelta);
			m_pSelectedObject->LateUpdate_Object();
			m_pSelectedObject->Render_Object();
		}
	}

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	if (ImGui::BeginTabItem("Monster"))
	{
		if (ImGui::Button("Blue_Beatle"))
		{
			m_eSelectedObjType = OBJ_SELECTED::BLUE_BEATLE;
			ResetSelectTarget();
			m_pSelectedObject = CBlueBeatle::Create(m_pGraphicDev);
		}

		if (ImGui::Button("Desert_Rino"))
		{
			m_eSelectedObjType = OBJ_SELECTED::DESERT_RHINO;
			ResetSelectTarget();
			m_pSelectedObject = CDesertRhino::Create(m_pGraphicDev);
		}

		if (ImGui::Button("Trash_Big"))
		{
			m_eSelectedObjType = OBJ_SELECTED::TRASH_BIG;
			ResetSelectTarget();
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
					ResetSelectTarget();
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

	CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::TERRAIN)->Find_GameObject(L"Terrain");
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

void CImGuiMgr::ResetSelectTarget()
{
	if (m_pSelectedObject)
	{
		Safe_Release(m_pSelectedObject);
		m_pSelectedObject = nullptr;
	}
	m_pTargetObject = nullptr;
}

void CImGuiMgr::CreateObj(OBJ_SELECTED _eSelected, _vec3& vHit)
{
	CGameObject* pCloneObj = nullptr;
	_vec3 vPos;
	switch (_eSelected)
	{
	case OBJ_SELECTED::BLUE_BEATLE :
		pCloneObj = CBlueBeatle::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_SELECTED::DESERT_RHINO:
		pCloneObj = CDesertRhino::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_SELECTED::TRASH_BIG:
		pCloneObj = CTrashBig::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(m_strObjNaming + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_SELECTED::TILE:
		pCloneObj = CTile::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Scale(m_vObjScale);
		pCloneObj->Get_TextureCom()->Set_Idx(m_pSelectedObject->Get_TextureCom()->Get_Idx());
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(m_strObjNaming + to_wstring(m_iObjNum++), pCloneObj);
		break;
	}

	CBoxCollider* pBoxCol = dynamic_cast<CBoxCollider*>(pCloneObj->Get_ColliderCom());

	if (pBoxCol != nullptr)
		pBoxCol->Set_Scale(m_vObjScale);

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

void CImGuiMgr::Input(const _float& fTimeDelta)
{

	_vec3 vPos;
	
	if (m_pTargetObject)
	{
		m_pTargetObject->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

		if (KEY_TAP(KEY::UP_ARROW))
		{
			vPos.z += 0.1f;
		}
		if (KEY_TAP(KEY::DOWN_ARROW))
		{
			vPos.z -= 0.1f;
		}
		if (KEY_TAP(KEY::LEFT_ARROW))
		{
			vPos.x -= 0.1f;
		}
		if (KEY_TAP(KEY::RIGHT_ARROW))
		{
			vPos.x += 0.1f;
		}
		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Q))
		{
			_vec3 vLook, vRight, vUp;
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);

			m_pTargetObject->Get_TransformCom()->RotationAxis(vLook, D3DXToRadian(90.f));
		}
		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::E))
		{
			_vec3 vLook, vRight, vUp;
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);

			m_pTargetObject->Get_TransformCom()->RotationAxis(vLook, D3DXToRadian(-90.f));
		}

		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::F))
		{
			m_vObjScale.z *= -1.f;
		}
		m_pTargetObject->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
	}

	
	_long		dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
	{
		if (KEY_HOLD(KEY::CTRL))
		{
			_float fFov = m_pToolScene->Get_MainCamera()->Get_Fov() - dwMouseMove * fTimeDelta;
			if (dwMouseMove > 0)
			{
				fFov = m_pToolScene->Get_MainCamera()->Get_Fov() + dwMouseMove * fTimeDelta * 0.1f;
				fFov < 1.f ? fFov = 1.f : fFov = fFov;
				m_pToolScene->Get_MainCamera()->Set_Fov(fFov);
			}
			else
			{
				fFov = m_pToolScene->Get_MainCamera()->Get_Fov() + dwMouseMove * fTimeDelta * 0.1f;
				fFov < 1.f ? fFov = 1.f : fFov = fFov;
				m_pToolScene->Get_MainCamera()->Set_Fov(fFov);
			}
		}	
	}

	if (KEY_TAP(KEY::O))
	{
		m_vObjScale.x += 1.f;
		m_vObjScale.y += 1.f;
		m_vObjScale.z += 1.f;
	}

	if (KEY_TAP(KEY::P))
	{
		m_vObjScale.x -= 1.f;
		m_vObjScale.y -= 1.f;
		m_vObjScale.z -= 1.f;

		if(m_vObjScale.x < 1.f)
			m_vObjScale.x = 1.f;
		if (m_vObjScale.y < 1.f)
			m_vObjScale.y = 1.f;
		if (m_vObjScale.z < 1.f)
			m_vObjScale.z = 1.f;
	}
	ObjPicking();
}

void CImGuiMgr::ObjPicking()
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
	{
		for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
		{
			if (i == (_uint)LAYER_TYPE::TERRAIN || i == (_uint)LAYER_TYPE::CAMERA)
				continue;

			const vector<CGameObject*>& vecObj = m_pToolScene->Get_Layer((LAYER_TYPE)i)->Get_GameObjectVec();
			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				_vec3 vHit;
				if (Engine::IsPicking(vecObj[j], &vHit))
				{	
					if (m_pSelectedObject != nullptr)
					{
						Safe_Release(m_pSelectedObject);
						m_pSelectedObject = nullptr;
					}

					if (m_pTargetObject != nullptr)
						m_pTargetObject = nullptr;

					m_pTargetObject = vecObj[j];
					m_vObjScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

					return;
				}
			}
		}
	}
}

void CImGuiMgr::Update_Help(const _float& fTimeDelta)
{
	ImGui::Begin(u8"Help");
	ImGui::Text(u8"����");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"Q : ��");
	ImGui::SameLine();
	ImGui::Text(u8"W : ��");
	ImGui::SameLine();
	ImGui::Text(u8"E : ��");

	ImGui::Text(u8"A : ��");
	ImGui::SameLine();
	ImGui::Text(u8"S : ��");
	ImGui::SameLine();
	ImGui::Text(u8"D : ��");

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"Ctrl + Q : Rotation CCW");
	ImGui::Text(u8"Ctrl + F : Flip");
	ImGui::Text(u8"Ctrl + E : Rotation CW");
	ImGui::Text(u8"Ctrl + �� : ZoomIn, ZoomOut");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"1. L_BUTTON : ��ġ");
	ImGui::Text(u8"2. R_BUTTON : ���");
	ImGui::Text(u8"3. R_BUTTON + MOVE : ī�޶� ȸ��");
	ImGui::Text(u8"4. Ctrl + L_BUTTON : ������Ʈ ����");
	
	ImGui::End();
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
			ImGui::InputFloat("##ScaleX", &m_vObjScale.x, 0.f, 0.5f, "%.3f");

			ImGui::Text("y : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ScaleY", &m_vObjScale.y, 0.f, 0.5f, "%.3f");

			ImGui::Text("z : ");
			ImGui::SameLine();
			ImGui::InputFloat("##ScaleZ", &m_vObjScale.z, 0.f, 0.5f, "%.3f");

			pTargetTransform->Set_Info(INFO_POS, &vPos);
			pTargetTransform->Set_Scale(m_vObjScale);
		}
	}
	ImGui::End();
}

void CImGuiMgr::Update_Hierachy(const _float& fTimeDelta)
{
	// â ����
	ImGui::Begin("Hierachy");
	if (nullptr != m_pToolScene)
	{
		ImGui::BeginListBox(" ", ImVec2(400, 350));
		for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
		{
			if (nullptr == m_pToolScene->Get_Layer((LAYER_TYPE)i))
				continue;

			const vector<CGameObject*>& vecObjects = m_pToolScene->Get_Layer((LAYER_TYPE)i)->Get_GameObjectVec();

			for (auto& iter : vecObjects)
			{
				string strName = std::string().assign(iter->Get_Name().begin(), iter->Get_Name().end());
				if (ImGui::Selectable((strName).c_str(), iter == m_pTargetObject))
				{
					m_pTargetObject = iter;
					m_vObjScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

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
