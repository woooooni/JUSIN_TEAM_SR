#include "../Include/stdafx.h"
#include "..\Header\Loading.h"
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


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{

}

unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->m_eID)
	{
	case Engine::SCENE_TYPE::LOADING:
		iFlag = pLoading->Load_Texture();
		break;
	case Engine::SCENE_TYPE::STAGE1:
		break;
	case Engine::SCENE_TYPE::STAGE2:
		break;
	case Engine::SCENE_TYPE::STAGE3:
		break;
	case Engine::SCENE_TYPE::TOOL:
		break;
	case Engine::SCENE_TYPE::SCENE_END:
		break;
	default:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

HRESULT CLoading::Ready_Loading(SCENE_TYPE eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eLoadingID;

	switch (m_eID)
	{
	case Engine::SCENE_TYPE::LOADING:
		break;
	case Engine::SCENE_TYPE::STAGE1:
		break;
	case Engine::SCENE_TYPE::STAGE2:
		break;
	case Engine::SCENE_TYPE::STAGE3:
		break;
	case Engine::SCENE_TYPE::TOOL:
		break;
	case Engine::SCENE_TYPE::SCENE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

_uint CLoading::Load_Texture()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90)), E_FAIL);

	m_bFinish = true;

	return 0;
}

_uint CLoading::Load_Stage1()
{
	return _uint();
}

_uint CLoading::Load_Obj_Data(wstring _strFolderPath)
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
		return E_FAIL;
	}


	DWORD	dwByte = 0;
	CGameObject* pObj;

	// data
	_uint iLayerType = 0, iObjId, iTextureIdx = 0;
	_vec3 vRight, vUp, vLook, vPos, vScale, vColliderScale;
	_bool bTextureExist;

	_uint iCount = 0;
	while (true)
	{
		ReadFile(hObjFile, &iLayerType, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		ReadFile(hObjFile, &iObjId, sizeof(_uint), &dwByte, nullptr);

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

		m_pLoadingScene->Get_Layer((LAYER_TYPE)iLayerType)->Add_GameObject(L"OBJ_" + to_wstring(iCount++), pObj);
	}

	CloseHandle(hObjFile);
	return S_OK;

}

_uint CLoading::Load_Terrain_Data(wstring _strFolderPath)
{
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
		return E_FAIL;
	}

	DWORD	dwByte = 0;
	CTerrain* pPreTerrain = dynamic_cast<CTerrain*>(m_pLoadingScene->Get_Layer(LAYER_TYPE::TERRAIN)->Find_GameObject(L"Terrain"));
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	if (pTerrain == nullptr || pPreTerrain == nullptr)
	{
		MessageBox(g_hWnd, _T("Get Terrain"), L"Fail", MB_OK);
		CloseHandle(hTerrainFile);
		return E_FAIL;
	}


	vector<CGameObject*>& vecTerrain = m_pLoadingScene->Get_Layer(LAYER_TYPE::TERRAIN)->Get_GameObjectVec();
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

	m_pLoadingScene->Get_Layer(LAYER_TYPE::TERRAIN)->Add_GameObject(L"Terrain", pTerrain);

	CloseHandle(hTerrainFile);

	return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE  eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

