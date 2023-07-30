#include "../Include/stdafx.h"
#include "..\Header\Scene_Tool.h"
#include "Export_Function.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera.h"
#include "CameraMgr.h"
#include "ImGuiMgr.h"
#include "UI_Pages.h"
#include "UI_AdventureBook.h"
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
#include "Grass.h"
#include "GameMgr.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "SkyBox.h"
#include "HitObj.h"
#include "HoleObj.h"
#include "BlockObj.h"
#include "BalpanObj.h"
#include "PushStone.h"
#include "BreakStone.h"
#include	"Catapult.h"


CScene_Tool::CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::TOOL)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

HRESULT CScene_Tool::Ready_Scene()
{
	FAILED_CHECK_RETURN(CImGuiMgr::GetInstance()->Ready_ImGuiMgr(g_hWnd, m_pGraphicDev), E_FAIL);
	__super::Ready_AllLayer();

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);
	

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	m_pGraphicDev->SetViewport(&vp);

	CImGuiMgr::GetInstance()->SetToolScene(this);    

	CLightMgr::GetInstance()->Ready_LightMgr(m_pGraphicDev);
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Get_LightInfo().Ambient = { 1.f, 1.f, 1.f, 1.f };
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Get_LightInfo().Diffuse = { 1.f, 1.f, 1.f, 1.f };

	

	return S_OK;
}

Engine::_int CScene_Tool::Update_Scene(const _float& fTimeDelta)
{
	_int		iResult = 0;
	CImGuiMgr::GetInstance()->Update_ImGui(fTimeDelta);
	for (auto& iter : m_mapLayer)
	{
		if (iter.first == LAYER_TYPE::MONSTER)
			iResult = iter.second->Update_Layer(0.f);
		else
			iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
	return 0;
}

void CScene_Tool::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
	
}

void CScene_Tool::Render_Scene()
{
	CImGuiMgr::GetInstance()->Render_ImGui();

	RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	m_pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));
}

void CScene_Tool::Free()
{
	__super::Free();
}

void CScene_Tool::Save_Data(wstring _strFolderPath)
{
	Save_Terrain_Data(_strFolderPath);
	Save_Obj_Data(_strFolderPath);
}

void CScene_Tool::Load_Data(wstring _strFolderPath)
{
	Load_Terrain_Data(_strFolderPath);
	Load_Obj_Data(_strFolderPath);
}




void CScene_Tool::Save_Terrain_Data(wstring _strFolderPath)
{
	// TODO::SAVE GROUND
	wstring strTerrainPath = _strFolderPath + L"/Terrain.dat";
	HANDLE	hTerrainFile = CreateFile(strTerrainPath.c_str(),		// 파일 경로와 파일 이름을 명시
		GENERIC_WRITE,			// 파일 접근 모드(GENERIC_WRITE : 쓰기 모드 GENERIC_READ : 읽기 모드)
		NULL,					// 공유 모드 설정(NULL을 지정하면 공유하지 않음)
		NULL,					// 보안 속성 설정(NULL로 기본 보안 속성 설정)
		CREATE_ALWAYS,			// 파일 옵션( CREATE_ALWAYS(쓰기 전용) : 파일이 없다면 생성, 있다면 덮어쓰기 옵션, OPEN_EXISTING(읽기 전용) : 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 등) : 아무런 속성이 없는 일반 파일 생성
		NULL);					// 생성될 파일의 속성을 제공한 템플릿, 우리는 안쓰니깐 NULL

	if (INVALID_HANDLE_VALUE == hTerrainFile)	// 파일 개방에 실패했다면
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_mapLayer.find(LAYER_TYPE::TERRAIN)->second->Find_GameObject(L"Terrain"));
	if (pTerrain == nullptr)
	{
		MessageBox(g_hWnd, _T("Get Terrain"), L"Fail", MB_OK);
		CloseHandle(hTerrainFile);
		return;
	}

	CTransform* pTransform = pTerrain->Get_TransformCom();
	CTexture* pTexture = pTerrain->Get_TextureCom();

	_vec3 vRight, vUp, vLook, vPos, vScale;
	_uint iTextureIdx = pTexture->Get_Idx();

	pTransform->Get_Info(INFO_RIGHT, &vRight);
	pTransform->Get_Info(INFO_UP, &vUp);
	pTransform->Get_Info(INFO_LOOK, &vLook);
	pTransform->Get_Info(INFO_POS, &vPos);

	vScale = pTransform->Get_Scale();

	WriteFile(hTerrainFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
	WriteFile(hTerrainFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
	WriteFile(hTerrainFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
	WriteFile(hTerrainFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
	WriteFile(hTerrainFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
	WriteFile(hTerrainFile, &iTextureIdx, sizeof(_uint), &dwByte, nullptr);


	CloseHandle(hTerrainFile);
	MessageBox(g_hWnd, _T("Save TerrainData success"), L"Success", MB_OK);
}

void CScene_Tool::Save_Obj_Data(wstring _strFolderPath)
{
	wstring strObjPath = _strFolderPath + L"/Obj.dat";
	HANDLE	hObjFile = CreateFile(strObjPath.c_str(),		// 파일 경로와 파일 이름을 명시
		GENERIC_WRITE,			// 파일 접근 모드(GENERIC_WRITE : 쓰기 모드 GENERIC_READ : 읽기 모드)
		NULL,					// 공유 모드 설정(NULL을 지정하면 공유하지 않음)
		NULL,					// 보안 속성 설정(NULL로 기본 보안 속성 설정)
		CREATE_ALWAYS,			// 파일 옵션( CREATE_ALWAYS(쓰기 전용) : 파일이 없다면 생성, 있다면 덮어쓰기 옵션, OPEN_EXISTING(읽기 전용) : 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 등) : 아무런 속성이 없는 일반 파일 생성
		NULL);					// 생성될 파일의 속성을 제공한 템플릿, 우리는 안쓰니깐 NULL

	if (INVALID_HANDLE_VALUE == hObjFile)	// 파일 개방에 실패했다면
	{
		MessageBox(g_hWnd, _T("Save File Failed"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE(i) == LAYER_TYPE::PLAYER
			|| LAYER_TYPE(i) == LAYER_TYPE::CAMERA
			|| LAYER_TYPE(i) == LAYER_TYPE::EFFECT
			|| LAYER_TYPE(i) == LAYER_TYPE::TERRAIN
			|| LAYER_TYPE(i) == LAYER_TYPE::INTERACTION_OBJ
			|| LAYER_TYPE(i) == LAYER_TYPE::UI)
			continue;

		const vector<CGameObject*>& vecObj = m_mapLayer[(LAYER_TYPE)i]->Get_GameObjectVec();

		for (size_t idx = 0; idx < vecObj.size(); ++idx)
		{
			CGameObject* pObj = vecObj[idx];

			if (nullptr == pObj)
				continue;
			// 0. LAYER_TYPE
			_uint iLayerType = i;
			WriteFile(hObjFile, &iLayerType, sizeof(_uint), &dwByte, nullptr);

			// 1. ObjId
			_uint iObjId = (_uint)pObj->GetObj_Id();
			WriteFile(hObjFile, &iObjId, sizeof(_uint), &dwByte, nullptr);

			if (iObjId == (_uint)OBJ_ID::GRASS)
			{
				_uint iGlassType = _uint(dynamic_cast<CGrass*>(pObj)->Get_Type());
				WriteFile(hObjFile, &iGlassType, sizeof(_uint), &dwByte, nullptr);
			}


			CTransform* pTransform = pObj->Get_TransformCom();
			CTexture* pTexture = pObj->Get_TextureCom();
			CBoxCollider* pCollider = dynamic_cast<CBoxCollider*>(pObj->Get_ColliderCom());
			

			// 3. Transform
			_vec3 vRight, vUp, vLook, vPos, vScale;

			pTransform->Get_Info(INFO_RIGHT, &vRight);
			pTransform->Get_Info(INFO_UP, &vUp);
			pTransform->Get_Info(INFO_LOOK, &vLook);
			pTransform->Get_Info(INFO_POS, &vPos);
			vScale = pTransform->Get_Scale();

			WriteFile(hObjFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hObjFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hObjFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hObjFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hObjFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
			
			// 4. Collider
			
			_vec3 vColliderScale = pCollider->Get_Scale();
			WriteFile(hObjFile, &vColliderScale, sizeof(_vec3), &dwByte, nullptr);
			
			// 5. Texture
			_bool bTextureExist = (pTexture != nullptr);
			WriteFile(hObjFile, &bTextureExist, sizeof(_bool), &dwByte, nullptr);

			if (bTextureExist)
			{
				_uint iTextureIdx = pTexture->Get_Idx();
				WriteFile(hObjFile, &iTextureIdx, sizeof(_uint), &dwByte, nullptr);
			}
		}
	}

	CloseHandle(hObjFile);
	MessageBox(g_hWnd, _T("Save success"), L"Success", MB_OK);

}

void CScene_Tool::Load_Terrain_Data(wstring _strFolderPath)
{
	// TODO::SAVE GROUND
	wstring strTerrainPath = _strFolderPath + L"/Terrain.dat";
	HANDLE	hTerrainFile = CreateFile(strTerrainPath.c_str(),		// 파일 경로와 파일 이름을 명시
		GENERIC_READ,			// 파일 접근 모드(GENERIC_WRITE : 쓰기 모드 GENERIC_READ : 읽기 모드)
		NULL,					// 공유 모드 설정(NULL을 지정하면 공유하지 않음)
		NULL,					// 보안 속성 설정(NULL로 기본 보안 속성 설정)
		OPEN_EXISTING,			// 파일 옵션( CREATE_ALWAYS(쓰기 전용) : 파일이 없다면 생성, 있다면 덮어쓰기 옵션, OPEN_EXISTING(읽기 전용) : 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 등) : 아무런 속성이 없는 일반 파일 생성
		NULL);					// 생성될 파일의 속성을 제공한 템플릿, 우리는 안쓰니깐 NULL

	if (INVALID_HANDLE_VALUE == hTerrainFile)	// 파일 개방에 실패했다면
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	CTerrain* pPreTerrain = dynamic_cast<CTerrain*>(m_mapLayer.find(LAYER_TYPE::TERRAIN)->second->Find_GameObject(L"Terrain"));
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	if (pTerrain == nullptr || pPreTerrain == nullptr)
	{
		MessageBox(g_hWnd, _T("Get Terrain"), L"Fail", MB_OK);
		CloseHandle(hTerrainFile);
		return;
	}

	
	vector<CGameObject*>& vecTerrain = m_mapLayer.find(LAYER_TYPE::TERRAIN)->second->Get_GameObjectVec();
	for (auto& iter = vecTerrain.begin(); iter != vecTerrain.end(); ++iter)
	{
		if (*iter == pPreTerrain)
		{
			vecTerrain.erase(iter);
			break;
		}		
	}

	Safe_Release(pPreTerrain);



	CTransform* pTransform = pTerrain->Get_TransformCom();
	CTexture* pTexture = pTerrain->Get_TextureCom();

	_vec3 vRight, vUp, vLook, vPos, vScale;
	_uint iTextureIdx;


	vScale = pTransform->Get_Scale();

	ReadFile(hTerrainFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hTerrainFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hTerrainFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hTerrainFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hTerrainFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hTerrainFile, &iTextureIdx, sizeof(_uint), &dwByte, nullptr);

	

	pTransform->Set_Info(INFO_RIGHT, &vRight);
	pTransform->Set_Info(INFO_UP, &vUp);
	pTransform->Set_Info(INFO_LOOK, &vLook);
	pTransform->Set_Info(INFO_POS, &vPos);
	pTransform->Set_Scale(vScale);

	pTexture->Set_Idx(iTextureIdx);

	m_mapLayer.find(LAYER_TYPE::TERRAIN)->second->Add_GameObject(L"Terrain", pTerrain);

	CloseHandle(hTerrainFile);
	MessageBox(g_hWnd, _T("Load success"), L"Success", MB_OK);
}

void CScene_Tool::Load_Obj_Data(wstring _strFolderPath)
{
	wstring strObjPath = _strFolderPath + L"/Obj.dat";
	HANDLE	hObjFile = CreateFile(strObjPath.c_str(),		// 파일 경로와 파일 이름을 명시
		GENERIC_READ,			// 파일 접근 모드(GENERIC_WRITE : 쓰기 모드 GENERIC_READ : 읽기 모드)
		NULL,					// 공유 모드 설정(NULL을 지정하면 공유하지 않음)
		NULL,					// 보안 속성 설정(NULL로 기본 보안 속성 설정)
		OPEN_EXISTING,			// 파일 옵션( CREATE_ALWAYS(쓰기 전용) : 파일이 없다면 생성, 있다면 덮어쓰기 옵션, OPEN_EXISTING(읽기 전용) : 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 등) : 아무런 속성이 없는 일반 파일 생성
		NULL);					// 생성될 파일의 속성을 제공한 템플릿, 우리는 안쓰니깐 NULL

	if (INVALID_HANDLE_VALUE == hObjFile)	// 파일 개방에 실패했다면
	{
		MessageBox(g_hWnd, _T("Save Obj File Failed"), L"Fail", MB_OK);
		return;
	}

	Clear_Layer();


	DWORD	dwByte = 0;
	CGameObject* pObj;

	// data
	_uint iLayerType = 0, iObjId, iTextureIdx = 0;
	_uint iGlassType = 0;
	_vec3 vRight, vUp, vLook, vPos, vScale, vColliderScale;
	_bool bTextureExist;

	_uint iCount = 0;
	while (true)
	{
		ReadFile(hObjFile, &iLayerType, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		ReadFile(hObjFile, &iObjId, sizeof(_uint), &dwByte, nullptr);

		if (iObjId == _uint(OBJ_ID::GRASS))
			ReadFile(hObjFile, &iGlassType, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hObjFile, &vRight, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &vUp, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &vLook, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &vColliderScale, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hObjFile, &bTextureExist, sizeof(_bool), &dwByte, nullptr);

		if (bTextureExist)
			ReadFile(hObjFile, &iTextureIdx, sizeof(_uint), &dwByte, nullptr);

		switch ((OBJ_ID)iObjId)
		{
		case OBJ_ID::BLUE_BEATLE:
			pObj = CBlueBeatle::Create(m_pGraphicDev);
			break;

		case OBJ_ID::RED_BEATLE:
			pObj = CRedBeatle::Create(m_pGraphicDev);
			break;

		case OBJ_ID::GREEN_BEATLE:
			pObj = CGreenBeatle::Create(m_pGraphicDev);
			break;

		case OBJ_ID::DESERT_RHINO:
			pObj = CDesertRhino::Create(m_pGraphicDev);
			break;

		case OBJ_ID::TRASH_BIG:
			pObj = CTrashBig::Create(m_pGraphicDev);
			break;

		case OBJ_ID::TRASH_SLIME:
			pObj = CTrashSlime::Create(m_pGraphicDev);
			break;

		case OBJ_ID::SPIT_CACTUS:
			pObj = CSpitCactus::Create(m_pGraphicDev);
			break;

		case OBJ_ID::MORTH_MAGE:
			pObj = CMothMage::Create(m_pGraphicDev);
			break;

		case OBJ_ID::ROLLING_BUG:
			pObj = CRollingBug::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, BUGCOLORTYPE::BLUE);
			break;

		case OBJ_ID::CUPA:
			pObj = CCupa::Create(m_pGraphicDev);
			break;

		case OBJ_ID::TILE:
			pObj = CTile::Create(m_pGraphicDev);
			break;

		case OBJ_ID::TREE:
			pObj = CTree::Create(m_pGraphicDev);
			break;

		case OBJ_ID::HOUSE:
			pObj = CHouse::Create(m_pGraphicDev);
			break;
		case OBJ_ID::PROP:
			pObj = CProp::Create(m_pGraphicDev);
			break;

		case OBJ_ID::GRASS:
			pObj = CGrass::Create(m_pGraphicDev, GRASS_TYPE(iGlassType));
			break;
		default:
			continue;
		}

		CTransform* pTransform = pObj->Get_TransformCom();
		CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(pObj->Get_ColliderCom());
		if (bTextureExist)
		{
			CTexture* pTexture = pObj->Get_TextureCom();
			pTexture->Set_Idx(iTextureIdx);
		}

		pTransform->Set_Info(INFO_RIGHT, &vRight);
		pTransform->Set_Info(INFO_UP, &vUp);
		pTransform->Set_Info(INFO_LOOK, &vLook);
		pTransform->Set_Info(INFO_POS, &vPos);
		pTransform->Set_Scale(vScale);

		pBoxCollider->Set_Scale(vColliderScale);

		m_mapLayer[(LAYER_TYPE)iLayerType]->Add_GameObject(L"OBJ_" + to_wstring(iCount++), pObj);
	}

	CloseHandle(hObjFile);
	MessageBox(g_hWnd, _T("Load Obj Data Success"), L"Success", MB_OK);
}

void CScene_Tool::Clear_Layer()
{
	for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE(i) == LAYER_TYPE::PLAYER
			|| LAYER_TYPE(i) == LAYER_TYPE::CAMERA
			|| LAYER_TYPE(i) == LAYER_TYPE::EFFECT
			|| LAYER_TYPE(i) == LAYER_TYPE::TERRAIN
			|| LAYER_TYPE(i) == LAYER_TYPE::UI
			|| LAYER_TYPE(i) == LAYER_TYPE::INTERACTION_OBJ)
			continue;

		m_mapLayer[(LAYER_TYPE)i]->Free();
	}
}


CScene_Tool* CScene_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Tool* pInstance = new CScene_Tool(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Tool_Scene Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CScene_Tool::Ready_Prototype()
{
	FAILED_CHECK_RETURN(CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev), E_FAIL);
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Player()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer), E_FAIL);
	m_pPlayer = pPlayer;

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Camera()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::CAMERA];

	Engine::CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pCamera), E_FAIL);

	/*CSkyBox* pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pSkyBox), E_FAIL);*/

	m_pCamera = pCamera;

	pCamera->Set_CameraState(CAMERA_STATE::TOOL);
	pCamera->Set_TargetObj(m_pPlayer);

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Terrrain()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::TERRAIN];

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Environment()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::ENVIRONMENT];

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	

	

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Effect()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::EFFECT];

	pLayer->Ready_Layer();
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_UI()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];

	pLayer->Ready_Layer();
	return S_OK;
}

