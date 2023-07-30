#include "..\Header\Scene_MoonForest1.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "UIMgr.h"
#include "JellyStone.h"
#include "BalpanObj.h"
#include "LightFlower.h"
#include "Door.h"
#include	"LightPuzzleBase.h"
#include	"LightPuzzlePiece.h"
#include	"LightPuzzleTerrain.h"
#include "RollingBug.h"
#include "Portal.h"
#include "UI_MapName.h"
#include "RabbitMgr.h"

CScene_MoonForest1::CScene_MoonForest1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MOON_FOREST1)
{
}

CScene_MoonForest1::~CScene_MoonForest1()
{
}

HRESULT CScene_MoonForest1::Ready_Scene()
{
	
	__super::Ready_AllLayer();
	FAILED_CHECK_RETURN(Ready_Event(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	Stop_Sound(CHANNELID::SOUND_BGM);
	Play_BGM(L"BGM_49_CentralArea2.wav", 0.5f);

	D3DLIGHT9 tLight;
	tLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tLight.Direction = { 0.0f, -1.0f, 1.0f };
	tLight.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	tLight.Diffuse = { 0.2f, 0.2f, 0.2f, 1.0f };
	tLight.Specular = { 0.2f, 0.2f, 0.2f, 1.0f };

	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Set_LightInfo(tLight);

	return S_OK;
}

_int CScene_MoonForest1::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	CRabbitMgr::GetInstance()->Update_Object(fTimeDelta);
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MoonForest1::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();
	__super::LateUpdate_Scene();
}

void CScene_MoonForest1::Render_Scene()
{
	RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));

	CUIMgr::GetInstance()->Render_UIMgr();
	CRabbitMgr::GetInstance()->Render_Object();
}

HRESULT CScene_MoonForest1::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Player()
{
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", CGameMgr::GetInstance()->Get_Player());
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vPos;
	vPos.x = 58.5f;
	vPos.y = 0.5f;
	vPos.z = 2.f;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);
	pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Environment()
{
	CGameObject* pDoor = CDoor::Create(m_pGraphicDev);
	dynamic_cast<CDoor*>(pDoor)->Set_Door(_vec3(54.0f, 2.0f, 72.0f), _vec3(4.0f, 6.0f, 1.5f));
	Add_Subscribe(21, pDoor);

	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::SILK_WORM);

	_vec3 vPos;
	vPos.x = 54.f;
	vPos.y = 0.5f;
	vPos.z = 72.5f;

	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Door", pDoor);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPotal", pPortal);

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Monster()
{
	CRollingBug* pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(64.f, 0.5f, 52.f), BUGCOLORTYPE::YELLOW);
	m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(58.f, 0.5f, 52.f), BUGCOLORTYPE::PINK);
	m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_InterationObj()
{
	/*CJellyStone* pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 0, { 62.f, 0.f, 45.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"JellyStone", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 0, { 65.f, 0.f, 45.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"JellyStone", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::BLUE, 0, { 68.f, 0.f, 45.f });

	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"JellyStone", pJel);*/


	CBalpanObj* pBal = CBalpanObj::Create(m_pGraphicDev, 20, { 61.f, 0.f, 50.f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Answer(JELLY_COLOR::YELLOW);
	NULL_CHECK_RETURN(pBal, E_FAIL);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);


	/*CLightFlower* pLight = CLightFlower::Create(m_pGraphicDev, pBal, 20, { 61.f, 0.f, 53.f });
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightFlower", pLight);*/


	CLightPuzzleTerrain* pTer = CLightPuzzleTerrain::Create(m_pGraphicDev, 4, 5, { 45.f, 0.01f, 45.f });
	NULL_CHECK_RETURN(pTer, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Ter", pTer);

	CLightPuzzleBase* pBase = CLightPuzzleBase::Create(m_pGraphicDev, 21, pTer->Get_TilePos(0, 2));
	NULL_CHECK_RETURN(pBase, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);


	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 22, pTer->Get_TilePos(2, 0), L"Vertical");
	NULL_CHECK_RETURN(pBase, E_FAIL);
	pBase->Reverse_Puzzle(false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 22, pTer->Get_TilePos(3, 2), L"Three");
	NULL_CHECK_RETURN(pBase, E_FAIL);
	pBase->Set_MakeLight();

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 22, pTer->Get_TilePos(3, 4), L"Corner");
	NULL_CHECK_RETURN(pBase, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	CLightPuzzlePiece* pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 43, 0, 43 }, L"Corner");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);
	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 44, 0, 43 }, L"Base");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);

	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 46, 0, 43 }, L"Horizon");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);

	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 47, 0, 43 }, L"Corner");
	NULL_CHECK_RETURN(pPiece, E_FAIL);
	pPiece->Reverse_Puzzle(true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);


	CRabbitMgr::GetInstance()->Ready_Object(m_pGraphicDev, 4, 4, { 60.f, 0.f, 38.f });



	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::MOON_FOREST1);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Event()
{
	EVENT* event = new EVENT;
	event->iEventNum = 20;

	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 21;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 22;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 23;

	FAILED_CHECK(Add_Event(event));
	return S_OK;
}

CScene_MoonForest1* CScene_MoonForest1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MoonForest1* pInstance = new CScene_MoonForest1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_MoonForest1::Free()
{
	__super::Free();
}
