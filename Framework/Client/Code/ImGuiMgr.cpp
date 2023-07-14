#include "ImGuiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "Scene_Tool.h"
#include "GameObject.h"
#include "BlueBeatle.h"
#include "RedBeatle.h"
#include "GreenBeatle.h"
#include "DesertRhino.h"
#include "TrashBig.h"
#include "Tile.h"
#include "Tree.h"
#include "TrashSlime.h"
#include "SpitCactus.h"
#include "MothMage.h"
#include "RollingBug.h"
#include "Cupa.h"
#include "SunGollem.h"
#include "SilkWorm.h"
#include "House.h"
#include "Prop.h"

IMPLEMENT_SINGLETON(CImGuiMgr)
CImGuiMgr::CImGuiMgr()
	: m_bEnabled(true)
	, m_pGraphicDev(nullptr)
	, m_hWnd(nullptr)
	, m_eMode(TOOL_MODE::OBJECT)
	, m_pToolScene(nullptr)
	, m_pTargetObject(nullptr)
	, m_pSelectedObject(nullptr)
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
	
	// 데이터 저장 경로는 여기를 수정하세요. 반드시 폴더를 먼저 생성해야합니다.
	m_strFolderPath = L"../Bin/Data/Test";

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
	
	// 창 생성
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
		ResetSelectTarget();
		m_pToolScene->Save_Data(m_strFolderPath);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		ResetSelectTarget();
		m_pToolScene->Load_Data(m_strFolderPath);
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		ResetSelectTarget();
		m_pToolScene->Clear_Layer();
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

	if (m_pSelectedObject != nullptr)
	{
		CGameObject* pTerrain = Engine::Get_Layer(LAYER_TYPE::TERRAIN)->Find_GameObject(L"Terrain");

		if (pTerrain == nullptr)
			return;

		_vec3 vHit, vPos;
		if (Engine::IsPicking(pTerrain, &vHit))
		{
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
			
			vHit.x = ceil(vHit.x * 2) / 2;
			vHit.y = vPos.y;
			vHit.z = ceil(vHit.z * 2)/ 2;

			m_pSelectedObject->Get_TransformCom()->Set_Info(INFO_POS, &vHit);
			if (KEY_TAP(KEY::LBTN))
			{
				CreateObj(vHit);
			}

			if (KEY_TAP(KEY::RBTN))
			{
				ResetSelectTarget();
				return;
			}
		}

		m_pSelectedObject->Update_Object(0.f);
		m_pSelectedObject->LateUpdate_Object();
		

		/*OBJ_TYPE eObjType = m_pSelectedObject->GetObj_Type();
		if (eObjType == OBJ_TYPE::OBJ_MONSTER || eObjType == OBJ_TYPE::OBJ_ENVIRONMENT)
			SetAutoY(m_pSelectedObject);*/
	}

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::BeginTabBar("Tab", tab_bar_flags);
	if (ImGui::BeginTabItem("Monster"))
	{
		if (ImGui::Button("Blue_Beatle"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CBlueBeatle::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Red_Beatle"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CRedBeatle::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Green_Beatle"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CGreenBeatle::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Desert_Rino"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CDesertRhino::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Trash_Big"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CTrashBig::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Trash_Slime"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CTrashSlime::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Spit_Cactus"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CSpitCactus::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Moth_Mage"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CMothMage::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("Rolling_Bug"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CRollingBug::Create(m_pGraphicDev, {0.f, 0.f, 0.f}, BUGCOLORTYPE::PINK);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
		}

		if (ImGui::Button("CUPA"))
		{
			ResetSelectTarget();
			m_pSelectedObject = CCupa::Create(m_pGraphicDev);
			SetScaleRatio(m_pSelectedObject);
			SetAutoY(m_pSelectedObject);
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
				}
			}
		}
			
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tree"))
	{
		CTexture* pTreeTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Tree"));
		if (pTreeTex != nullptr)
		{
			for (size_t i = 0; i < pTreeTex->Get_Size(); ++i)
			{
				if (i % 4 != 0)
					ImGui::SameLine();

				if (ImGui::ImageButton(pTreeTex->Get_TextureVec()[i], ImVec2(50.f, 50.f)))
				{
					ResetSelectTarget();
					m_pSelectedObject = CTree::Create(m_pGraphicDev);
					m_pSelectedObject->Get_TextureCom()->Set_Idx(i);
					SetScaleRatio(m_pSelectedObject);
					SetAutoY(m_pSelectedObject);
				}
			}
		}
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("House"))
	{
		CTexture* pHouseTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_House"));
		if (pHouseTex != nullptr)
		{
			for (size_t i = 0; i < pHouseTex->Get_Size(); ++i)
			{
				if (i % 4 != 0)
					ImGui::SameLine();

				if (ImGui::ImageButton(pHouseTex->Get_TextureVec()[i], ImVec2(50.f, 50.f)))
				{
					ResetSelectTarget();
					m_pSelectedObject = CHouse::Create(m_pGraphicDev);
					m_pSelectedObject->Get_TextureCom()->Set_Idx(i);
					SetScaleRatio(m_pSelectedObject);
					SetAutoY(m_pSelectedObject);
				}
			}
		}
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Prop"))
	{
		CTexture* pPropTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Prop"));
		if (pPropTex != nullptr)
		{
			for (size_t i = 0; i < pPropTex->Get_Size(); ++i)
			{
				if (i % 4 != 0)
					ImGui::SameLine();

				if (ImGui::ImageButton(pPropTex->Get_TextureVec()[i], ImVec2(50.f, 50.f)))
				{
					ResetSelectTarget();
					m_pSelectedObject = CProp::Create(m_pGraphicDev);
					m_pSelectedObject->Get_TextureCom()->Set_Idx(i);
					SetScaleRatio(m_pSelectedObject);
					SetAutoY(m_pSelectedObject);
				}
			}
		}
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

void CImGuiMgr::PasteObj()
{
}

void CImGuiMgr::SetAutoY(CGameObject* pObj)
{
	if (pObj)
	{
		CTransform* pTransform = pObj->Get_TransformCom();
		
		_vec3 vPos;
		_vec3 vScale = pTransform->Get_Scale();
		_float fHeight;

		fHeight = vScale.y / 2.f;


		pTransform->Get_Info(INFO_POS, &vPos);
		vPos.y = fHeight;
		pTransform->GetOwner()->Set_MinHeight(fHeight);
		pTransform->Set_Info(INFO_POS, &vPos);
	}
}

void CImGuiMgr::SetScaleRatio(CGameObject* pObj)
{

	CTexture* pTexture = pObj->Get_TextureCom();
	if (pTexture)
	{
		const D3DXIMAGE_INFO& tImageInfo = pTexture->Get_TextureDesc(pTexture->Get_Idx());
		_vec3 vScale = pObj->Get_TransformCom()->Get_Scale();

		vScale.x = _float(tImageInfo.Width) / tImageInfo.Width;
		vScale.y = _float(tImageInfo.Height) / tImageInfo.Width;

		pObj->Get_TransformCom()->Set_Scale(vScale);
		return;
	}
	else
	{
		CAnimator* pAnimator = pObj->Get_AnimatorCom();
		_vec3 vScale = pObj->Get_TransformCom()->Get_Scale();

		const D3DXIMAGE_INFO& tImageInfo = pAnimator->GetCurrAnimation()->Get_TextureDesc(0);

		vScale.x = _float(tImageInfo.Width) / tImageInfo.Width;
		vScale.y = _float(tImageInfo.Height) / tImageInfo.Width;

		pObj->Get_TransformCom()->Set_Scale(vScale);
		return;
	}
}

void CImGuiMgr::CreateObj(_vec3& vHit)
{
	CGameObject* pCloneObj = nullptr;

	if (nullptr == m_pSelectedObject)
		return;

	switch (m_pSelectedObject->GetObj_Id())
	{
	case OBJ_ID::BLUE_BEATLE :
		pCloneObj = CBlueBeatle::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"BlueBeatle" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::RED_BEATLE:
		pCloneObj = CRedBeatle::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"RedBeatle" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::GREEN_BEATLE:
		pCloneObj = CGreenBeatle::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"GreenBeatle" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::DESERT_RHINO:
		pCloneObj = CDesertRhino::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"DesertRhino" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::TRASH_BIG:
		pCloneObj = CTrashBig::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"TrashBig" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::TRASH_SLIME:
		pCloneObj = CTrashSlime::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"TrashSlime" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::SPIT_CACTUS:
		pCloneObj = CSpitCactus::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"SpitCactus" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::MORTH_MAGE:
		pCloneObj = CMothMage::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"MorthMage" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::ROLLING_BUG:
		pCloneObj = CRollingBug::Create(m_pGraphicDev, {0.f, 0.f, 0.f}, BUGCOLORTYPE::BLUE);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"RollingBug" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::CUPA:
		pCloneObj = CCupa::Create(m_pGraphicDev);
		Engine::Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"Cupa" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::TILE:
		pCloneObj = CTile::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Scale(m_pSelectedObject->Get_TransformCom()->Get_Scale());
		pCloneObj->Get_TextureCom()->Set_Idx(m_pSelectedObject->Get_TextureCom()->Get_Idx());
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Tile_" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::TREE:
		pCloneObj = CTree::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Scale(m_pSelectedObject->Get_TransformCom()->Get_Scale());
		pCloneObj->Get_TextureCom()->Set_Idx(m_pSelectedObject->Get_TextureCom()->Get_Idx());
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Tree_" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::HOUSE:
		pCloneObj = CHouse::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Scale(m_pSelectedObject->Get_TransformCom()->Get_Scale());
		pCloneObj->Get_TextureCom()->Set_Idx(m_pSelectedObject->Get_TextureCom()->Get_Idx());
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"House" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	case OBJ_ID::PROP:
		pCloneObj = CProp::Create(m_pGraphicDev);
		pCloneObj->Get_TransformCom()->Set_Scale(m_pSelectedObject->Get_TransformCom()->Get_Scale());
		pCloneObj->Get_TextureCom()->Set_Idx(m_pSelectedObject->Get_TextureCom()->Get_Idx());
		Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Prop" + to_wstring(m_iObjNum++), pCloneObj);
		break;

	default :
		return;
	}

	_vec3 vPos;
	_vec3 vRight, vUp, vLook;
	_vec3 vScale;

	m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
	m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);
	m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);

	vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();


	// 회전 적용
	pCloneObj->Get_TransformCom()->Set_Info(INFO_RIGHT, &vRight);
	pCloneObj->Get_TransformCom()->Set_Info(INFO_UP, &vUp);
	pCloneObj->Get_TransformCom()->Set_Info(INFO_LOOK, &vLook);

	// 포지션 적용.
	m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	pCloneObj->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	pCloneObj->Set_MinHeight(m_pSelectedObject->Get_MinHeight());

	// 스케일 적용
	pCloneObj->Get_TransformCom()->Set_Scale(vScale);


	CBoxCollider* pBoxCol = dynamic_cast<CBoxCollider*>(pCloneObj->Get_ColliderCom());
	if(nullptr != pBoxCol)
		pBoxCol->Set_Scale(vScale);
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
		if (KEY_TAP(KEY::DEL))
		{
			DeleteObj();
			ResetSelectTarget();
			return;
		}

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
			_vec3 vLook, vRight, vUp;
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
			m_pTargetObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);

			m_pTargetObject->Get_TransformCom()->RotationAxis(vUp, D3DXToRadian(180.f));
		}

		if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET)) // '['
		{
			_vec3 vScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

			vScale.x -= 1.f;
			vScale.y -= 1.f;
			vScale.z -= 1.f;

			if (vScale.x < 1.f)
				vScale.x = 1.f;
			if (vScale.y < 1.f)
				vScale.y = 1.f;
			if (vScale.z < 1.f)
				vScale.z = 1.f;

			vScale.x = _int(vScale.x);
			vScale.y = _int(vScale.y);
			vScale.z = _int(vScale.z);

			m_pTargetObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pTargetObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

		}

		if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET)) // ']'
		{
			_vec3 vScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

			vScale.x += 1.0f;
			vScale.y += 1.0f;
			vScale.z += 1.0f;

			vScale.x = _int(vScale.x);
			vScale.y = _int(vScale.y);
			vScale.z = _int(vScale.z);

			m_pTargetObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pTargetObject->Get_ColliderCom());

			if(nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if(m_pTargetObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pTargetObject);
		}

		if (KEY_HOLD(KEY::O))
		{
			_vec3 vScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

			vScale.x -= 1.0f;
			vScale.y -= 1.0f;
			vScale.z -= 1.0f;

			if (vScale.x < 0.1f)
				vScale.x = 0.1f;

			if (vScale.y < 0.1f)
				vScale.y = 0.1f;

			if (vScale.z < 0.1f)
				vScale.z = 0.1f;

			m_pTargetObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pTargetObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if (m_pTargetObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pTargetObject);
		}

		if (KEY_HOLD(KEY::P))
		{
			_vec3 vScale = m_pTargetObject->Get_TransformCom()->Get_Scale();

			vScale.x += 1.f;
			vScale.y += 1.f;
			vScale.z += 1.f;

			m_pTargetObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pTargetObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if (m_pTargetObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pTargetObject);
		}

		m_pTargetObject->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
	}
	else if (m_pSelectedObject)
	{
		if (KEY_TAP(KEY::ESC))
		{
			ResetSelectTarget();
			return;
		}

		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Q))
		{
			_vec3 vLook, vRight, vUp;
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);

			m_pSelectedObject->Get_TransformCom()->RotationAxis(vLook, D3DXToRadian(90.f));
		}
		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::E))
		{
			_vec3 vLook, vRight, vUp;
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);
			m_pSelectedObject->Get_TransformCom()->Get_Info(INFO_UP, &vUp);

			m_pSelectedObject->Get_TransformCom()->RotationAxis(vLook, D3DXToRadian(-90.f));
		}

		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::F))
		{
			_vec3 vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();
			vScale.x *= -1.f;
			m_pSelectedObject->Get_TransformCom()->Set_Scale(vScale);
		}

		if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET)) // '['
		{
			_vec3 vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();

			vScale.x -= 1.f;
			vScale.y -= 1.f;
			vScale.z -= 1.f;

			if (vScale.x < 1.f)
				vScale.x = 1.f;
			if (vScale.y < 1.f)
				vScale.y = 1.f;
			if (vScale.z < 1.f)
				vScale.z = 1.f;

			m_pSelectedObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pSelectedObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if (m_pSelectedObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pSelectedObject);
		}

		if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET)) // ']'
		{
			_vec3 vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();

			vScale.x += 1.0f;
			vScale.y += 1.0f;
			vScale.z += 1.0f;

			m_pSelectedObject->Get_TransformCom()->Set_Scale(vScale);
			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pSelectedObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if (m_pSelectedObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pSelectedObject);
		}


		if (KEY_HOLD(KEY::O))
		{
			_vec3 vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();

			vScale.x -= 1.f;
			vScale.y -= 1.f;
			vScale.z -= 1.f;

			if (vScale.x < 1.f)
				vScale.x = 1.f;
			if (vScale.y < 1.f)
				vScale.y = 1.f;
			if (vScale.z < 1.f)
				vScale.z = 1.f;

			m_pSelectedObject->Get_TransformCom()->Set_Scale(vScale);

			CBoxCollider* pCollider = dynamic_cast<CBoxCollider*>(m_pSelectedObject->Get_ColliderCom());
			if (nullptr != pCollider)
				pCollider->Set_Scale(vScale);

			if (m_pSelectedObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pSelectedObject);
		}

		if (KEY_HOLD(KEY::P))
		{
			_vec3 vScale = m_pSelectedObject->Get_TransformCom()->Get_Scale();

			vScale.x += 1.f;
			vScale.y += 1.f;
			vScale.z += 1.f;

			m_pSelectedObject->Get_TransformCom()->Set_Scale(vScale);

			CBoxCollider* pCollider = dynamic_cast<CBoxCollider*>(m_pSelectedObject->Get_ColliderCom());
			if(nullptr != pCollider)
				pCollider->Set_Scale(vScale);

			if (m_pSelectedObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pSelectedObject);
		}

	}

	if (KEY_TAP(KEY::F1))
	{
		if (CAMERA_STATE::GAME == m_pToolScene->Get_MainCamera()->Get_CameraState())
		{
			m_pToolScene->Get_MainCamera()->Set_TargetObj(nullptr);
			m_pToolScene->Get_MainCamera()->Set_CameraState(CAMERA_STATE::TOOL);
			ResetSelectTarget();
		}
		else
		{
			m_pToolScene->Get_MainCamera()->Set_TargetObj((CGameObject*)m_pToolScene->Get_Player());
			m_pToolScene->Get_MainCamera()->Set_CameraState(CAMERA_STATE::GAME);
			ResetSelectTarget();
		}
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
					ResetSelectTarget();
					m_pTargetObject = vecObj[j];

					return;
				}
			}
		}
	}
}

void CImGuiMgr::Update_Help(const _float& fTimeDelta)
{
	ImGui::Begin(u8"Help");
	ImGui::Text(u8"도움말");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"Q : 상");
	ImGui::SameLine();
	ImGui::Text(u8"W : 앞");
	ImGui::SameLine();
	ImGui::Text(u8"E : 하");

	ImGui::Text(u8"A : 왼");
	ImGui::SameLine();
	ImGui::Text(u8"S : 뒤");
	ImGui::SameLine();
	ImGui::Text(u8"D : 우");
	ImGui::Text(u8"O, P : 확대 / 축소");
	ImGui::Text(u8"[ , ] : 세부 크기 조정");

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"Ctrl + Q : Rotation CCW");
	ImGui::Text(u8"Ctrl + F : Flip");
	ImGui::Text(u8"Ctrl + E : Rotation CW");
	ImGui::Text(u8"Ctrl + 휠 : ZoomIn, ZoomOut");

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text(u8"1. L_BUTTON : 배치");
	ImGui::Text(u8"2. R_BUTTON : 취소");
	ImGui::Text(u8"3. R_BUTTON + MOVE : 카메라 회전");
	ImGui::Text(u8"4. Ctrl + L_BUTTON : 오브젝트 선택");
	
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
			_vec3 vPos, vScale, vRight, vUp, vLook;
			pTargetTransform->Get_Info(INFO_POS, &vPos);
			vScale = pTargetTransform->Get_Scale();


			_float fPos[3] = { vPos.x, vPos.y, vPos.z };
			_float fRotation[3] = { 0.f, 0.f, 0.f };
			_float fScale[3] = { vScale.x, vScale.y, vScale.z };

			ImGui::SameLine();

			string strName = std::string().assign(m_pTargetObject->Get_Name().begin(), m_pTargetObject->Get_Name().end());
			ImGui::Text(strName.c_str());

			ImGui::Text("");
			ImGui::Text("");

			ImGui::Text("Transform");

			// Position
			ImGui::Text("");
			ImGui::Text("Position");
			ImGui::Text("x\t\t\ty\t\t\tz");

			ImGui::DragFloat3("##ObjPos", fPos, 0.1f);

			// Scale
			// Position
			ImGui::Text("");
			ImGui::Text("Scale");
			ImGui::Text("x\t\t\ty\t\t\tz");

			ImGui::DragFloat3("##ObjScale", fScale, 0.1f, 1.f, 100.f);

			vPos.x = fPos[0];
			vPos.y = fPos[1];
			vPos.z = fPos[2];

			vScale.x = fScale[0];
			vScale.y = fScale[1];
			vScale.z = fScale[2];

			if (vScale.x < 0.1f)
				vScale.x = 1.f;
			if (vScale.y < 0.1f)
				vScale.y = 1.f;
			if (vScale.z < 0.1f)
				vScale.z = 1.f;


			pTargetTransform->Set_Info(INFO_POS, &vPos);
			pTargetTransform->Set_Scale(vScale);

			CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pTargetObject->Get_ColliderCom());
			if (nullptr != pBoxCollider)
				pBoxCollider->Set_Scale(vScale);

			if (m_pTargetObject->GetObj_Id() != OBJ_ID::TILE)
				SetAutoY(m_pTargetObject);
		}
	}
	ImGui::End();
}

void CImGuiMgr::Update_Hierachy(const _float& fTimeDelta)
{
	const string STR_LAYER_TYPE[(_uint)LAYER_TYPE::LAYER_END]{ "CAMERA", "PLAYER", "TERRAIN", "ENVIRONMENT", "MONSTER", "EFFECT", "UI" };

	// 창 생성
	ImGui::Begin("Hierachy");
	if (nullptr != m_pToolScene)
	{
		ImGui::BeginListBox(" ", ImVec2(400, 350));
		for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
		{
			if (nullptr == m_pToolScene->Get_Layer((LAYER_TYPE)i))
				continue;
			ImGui::SeparatorText(STR_LAYER_TYPE[i].c_str());
			const vector<CGameObject*>& vecObjects = m_pToolScene->Get_Layer((LAYER_TYPE)i)->Get_GameObjectVec();
			for (auto& iter : vecObjects)
			{
				string strName = std::string().assign(iter->Get_Name().begin(), iter->Get_Name().end());
				if (ImGui::Selectable((strName).c_str(), iter == m_pTargetObject))
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
			}
			ImGui::SeparatorText("");
			ImGui::Text("");
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
