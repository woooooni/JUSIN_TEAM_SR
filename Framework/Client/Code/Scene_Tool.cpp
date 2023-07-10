#include "../Include/stdafx.h"
#include "..\Header\Scene_Tool.h"
#include "Export_Function.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera.h"
#include "CameraMgr.h"
#include "ImGuiMgr.h"


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
	__super::Ready_AllLayer();
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer(LAYER_TYPE::ENVIRONMENT), E_FAIL);
	

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	m_pGraphicDev->SetViewport(&vp);

	CImGuiMgr::GetInstance()->SetToolScene(this);    

	return S_OK;
}

Engine::_int CScene_Tool::Update_Scene(const _float& fTimeDelta)
{
	_int		iResult = 0;
	CImGuiMgr::GetInstance()->Update_ImGui(fTimeDelta);
	for (auto& iter : m_mapLayer)
	{
		if (iter.first == LAYER_TYPE::MONSTER)
		{
			for (_uint i = 0; i < iter.second->Get_GameObjectVec().size(); ++i)
				iter.second->Get_GameObjectVec()[i]->Set_Billboard();

			iResult = iter.second->Update_Layer(0.f);
		}
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
	MessageBox(g_hWnd, _T("Save success"), L"Success", MB_OK);
}

void CScene_Tool::Save_Obj_Data(wstring _strFolderPath)
{
	// TODO::SAVE GROUND
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
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE(i) == LAYER_TYPE::BACK_GROUND 
			|| LAYER_TYPE(i) == LAYER_TYPE::CAMERA 
			|| LAYER_TYPE(i) == LAYER_TYPE::EFFECT)
			continue;

		const vector<CGameObject*> vecObj = m_mapLayer[(LAYER_TYPE)i]->Get_GameObjectVec();
		for (size_t idx = 0; idx < vecObj.size(); ++idx)
		{
			CTransform* pTransform = vecObj[idx]->Get_TransformCom();
			if (pTransform != nullptr)
			{
				_vec3 vRight, vUp, vLook, vPos, vScale;
				pTransform->Get_Info(INFO_RIGHT, &vRight);
				pTransform->Get_Info(INFO_UP, &vUp);
				pTransform->Get_Info(INFO_LOOK, &vLook);
				pTransform->Get_Info(INFO_POS, &vPos);
				vScale = pTransform->Get_Scale();


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
}

void CScene_Tool::Clear_Layer()
{
}


HRESULT CScene_Tool::Ready_Layer(LAYER_TYPE _eType)
{
	Engine::CLayer* pLayerCamera = m_mapLayer.find(LAYER_TYPE::CAMERA)->second;
	Engine::CLayer* pLayerPlayer = m_mapLayer.find(LAYER_TYPE::PLAYER)->second;
	Engine::CLayer* pLayerEnv = m_mapLayer.find(LAYER_TYPE::ENVIRONMENT)->second;
	Engine::CLayer* pLayerTerrain = m_mapLayer.find(LAYER_TYPE::TERRAIN)->second;

	NULL_CHECK_RETURN(pLayerCamera, E_FAIL);
	NULL_CHECK_RETURN(pLayerPlayer, E_FAIL);
	NULL_CHECK_RETURN(pLayerEnv, E_FAIL);
	NULL_CHECK_RETURN(pLayerTerrain, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayerTerrain->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayerPlayer->Add_GameObject(L"Player", pPlayer), E_FAIL);
	m_pPlayer = pPlayer;

	// Camera
	Engine::CCamera * pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayerCamera->Add_GameObject(L"MainCamera", pCamera), E_FAIL);
	m_pCamera = pCamera;


	CImGuiMgr::GetInstance()->Set_Target(pPlayer);
	pCamera->Set_CameraState(CAMERA_STATE::TOOL);
	pCamera->Set_TargetObj(pPlayer);

	return S_OK;
}


CScene_Tool* CScene_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Tool* pInstance = new CScene_Tool(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CScene_Tool::Ready_Prototype()
{
	return S_OK;
}

