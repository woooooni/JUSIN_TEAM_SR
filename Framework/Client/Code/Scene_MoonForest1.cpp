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
#include "DrawingEnter.h"
#include "DefaultItem.h"
#include "TriggerObj.h"
#include "Particle_MapCircle.h"

CScene_MoonForest1::CScene_MoonForest1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MOON_FOREST1)
{
}

CScene_MoonForest1::~CScene_MoonForest1()
{
}

_bool	CScene_MoonForest1::m_bPlayedCutScene = false;

HRESULT CScene_MoonForest1::Ready_Scene()
{
	
	__super::Ready_AllLayer();

	D3DLIGHT9 tFlowerLight;
	ZeroMemory(&tFlowerLight, sizeof(D3DLIGHT9));

	tFlowerLight.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	tFlowerLight.Ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
	tFlowerLight.Diffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	tFlowerLight.Specular = { 0.5f, 0.5f, 0.5f, 0.5f };
	tFlowerLight.Range = 50.0f;
	tFlowerLight.Attenuation0 = 0.01f; // 상수 감쇠 계수
	tFlowerLight.Attenuation1 = 0.06f; // 선형 감쇠 계수
	tFlowerLight.Attenuation2 = 0.03f; // 이차 감쇠 계수


	Ready_Light(m_pGraphicDev, &tFlowerLight, (_uint)LIGHT_TYPE::LIGHT_FLOWER1);
	Ready_Light(m_pGraphicDev, &tFlowerLight, (_uint)LIGHT_TYPE::LIGHT_FLOWER2);



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
	Play_BGM(L"BGM_49_CentralArea2.wav", 0.3f);
	//BGM_14_MoonForestFieldHeavy.wav 추천 (49번은 튜토리얼 테마랑 똑같음)
	D3DLIGHT9 tLight;
	tLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tLight.Direction = { 0.0f, -1.0f, 1.0f };
	tLight.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	tLight.Diffuse = { 0.2f, 0.2f, 0.2f, 1.0f };
	tLight.Specular = { 0.2f, 0.2f, 0.2f, 1.0f };

	CLightMgr::GetInstance()->Reset_Light();
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Set_LightInfo(tLight);

	return S_OK;
}

_int CScene_MoonForest1::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MoonForest1::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();
	__super::LateUpdate_Scene();

	auto iter = m_mapLayer[LAYER_TYPE::MONSTER]->Find_GameObject(L"RollingBug");

	if (!iter && !m_bPlayedCutScene)
	{
		CCutSceneMgr::GetInstance()->Set_EventNum(28);
		CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE);
		m_bPlayedCutScene = true;
	}
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
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.x = 58.5f;
	vPos.z = 2.f;
	CGameMgr::GetInstance()->Get_Player()->Set_BalloonFly(false);
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
	Add_Subscribe(31, pDoor);

	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::SILK_WORM);

	_vec3 vPos;
	vPos.x = 54.f;
	vPos.y = 0.5f;
	vPos.z = 72.5f;

	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Door", pDoor);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPotal", pPortal);

	for (_uint i = 0; i < 300; ++i)
	{
		CParticle_MapCircle* pParticle = CParticle_MapCircle::Create(Engine::Get_Device());
		NULL_CHECK_RETURN(pParticle, E_FAIL);
		pParticle->Random_Particle(_vec3(30.0f, 10.0f, 30.0f), 300, (rand() % 4) * 85, (rand() % 4) * 85, (rand() % 4) * 85, 255);
		m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MapCircle", pParticle);
	}

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();
	return S_OK;
}

HRESULT CScene_MoonForest1::Ready_Layer_Monster()
{
	CRollingBug* pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(58.f, 0.5f, 17.5f), BUGCOLORTYPE::YELLOW);
	m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(60.f, 0.5f, 17.5f), BUGCOLORTYPE::PINK);
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


	CBalpanObj* pBal = CBalpanObj::Create(m_pGraphicDev, 20, { 97.5f, 0.f, 70.f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Answer(JELLY_COLOR::GREEN);
	NULL_CHECK_RETURN(pBal, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);


	pBal = CBalpanObj::Create(m_pGraphicDev, 22, { 100.5f, 0.f, 70.f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Answer(JELLY_COLOR::GREEN);
	NULL_CHECK_RETURN(pBal, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);


	pBal = CBalpanObj::Create(m_pGraphicDev, 23, { 103.5f, 0.f, 70.f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Answer(JELLY_COLOR::BLUE);
	NULL_CHECK_RETURN(pBal, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);

	pBal = CBalpanObj::Create(m_pGraphicDev, 29, { 54.f, 0.f, 12.5f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	NULL_CHECK_RETURN(pBal, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);
	
	CLightFlower* pLFlower = CLightFlower::Create(m_pGraphicDev, pBal, 29, { 54.f, 0.f, 17.5f });
	NULL_CHECK_RETURN(pLFlower, E_FAIL);
	pLFlower->Set_LightType(LIGHT_TYPE::LIGHT_FLOWER1);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightFlower", pLFlower);


	pBal = CBalpanObj::Create(m_pGraphicDev, 30, { 62.f, 0.f, 12.5f });
	pBal->Set_Static();
	pBal->Set_TargName(L"Jelly");
	NULL_CHECK_RETURN(pBal, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBal);



	pLFlower = CLightFlower::Create(m_pGraphicDev, pBal, 30, { 62.f, 0.f, 17.5f });
	NULL_CHECK_RETURN(pLFlower, E_FAIL);
	pLFlower->Set_LightType(LIGHT_TYPE::LIGHT_FLOWER2);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightFlower", pLFlower);




	CJellyStone* pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 20, { 97.5f, 0.f, 62.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 20, { 97.5f, 0.f, 64.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 20, { 97.5f, 0.f, 66.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);


	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 20, { 100.5f, 0.f, 62.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 20, { 100.5f, 0.f, 64.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);


	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 20, { 100.5f, 0.f, 66.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 20, { 103.5f, 0.f, 62.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 20, { 103.5f, 0.f, 64.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 20, { 103.5f, 0.f, 66.5f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);



	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::BLUE, 20, { 54.f, 0.f, 15.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::RED, 20, { 58.f, 0.f, 15.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::GREEN, 20, { 62.f, 0.f, 15.f });
	NULL_CHECK_RETURN(pJel, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);



	/*CLightFlower* pLight = CLightFlower::Create(m_pGraphicDev, pBal, 20, { 61.f, 0.f, 53.f });
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightFlower", pLight);*/


	CLightPuzzleTerrain* pTer = CLightPuzzleTerrain::Create(m_pGraphicDev, 4, 5, { 27.5, 0.01f, 40.5f });
	NULL_CHECK_RETURN(pTer, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Ter", pTer);

	CLightPuzzleBase* pBase = CLightPuzzleBase::Create(m_pGraphicDev, 21, pTer->Get_TilePos(0, 2));
	NULL_CHECK_RETURN(pBase, E_FAIL);
	pBase->Set_CutSceneType(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);


	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, pTer->Get_TilePos(2, 0), L"Vertical");
	NULL_CHECK_RETURN(pBase, E_FAIL);
	pBase->Reverse_Puzzle(false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, pTer->Get_TilePos(3, 2), L"Three");
	NULL_CHECK_RETURN(pBase, E_FAIL);
	pBase->Set_MakeLight();

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	pBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, pTer->Get_TilePos(3, 4), L"Corner");
	NULL_CHECK_RETURN(pBase, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Base", pBase);

	CLightPuzzlePiece* pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 27.5f, 0.f, 38.5f }, L"Corner");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);
	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 28.5f, 0.f, 38.5f }, L"Base");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);

	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 29.5f, 0.f, 38.5f }, L"Horizon");
	NULL_CHECK_RETURN(pPiece, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);

	pPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 30.5f, 0.f, 38.5f }, L"Corner");
	NULL_CHECK_RETURN(pPiece, E_FAIL);
	pPiece->Reverse_Puzzle(true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"LightPuzzle_Piece", pPiece);

	CDrawingEnter* pDraw = CDrawingEnter::Create(m_pGraphicDev, CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_BUGSTATUE));
	pDraw->Get_TransformCom()->Set_Pos(&_vec3(25.f, 0.f, 42.f));

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"DrawPoint", pDraw);

	CTriggerObj* pTrig = CTriggerObj::Create(m_pGraphicDev, { -10, -10, -10 });
	pTrig->Set_EventTrigger(
		27,
		[]()
		{
			CCutSceneMgr::GetInstance()->Set_EventNum(31);
			CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MOON_FOREST_DOOR);
		}
	);
	pTrig->Set_Once();

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);

	pTrig = CTriggerObj::Create(m_pGraphicDev, { -10, -10, -10 });
	pTrig->Set_EventTrigger(
		26,
		[]()
		{
			CCutSceneMgr::GetInstance()->Set_EventNum(32);
			CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE);
		}
	);
	pTrig->Set_Once();

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);

	/*pTrig = CTriggerObj::Create(m_pGraphicDev, {-10, -10, -10});
	pTrig->Set_EventTrigger(
		21,
		[]()
		{
			Check_Event_Start(27);
		}
	);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);*/



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

	event = new EVENT;
	event->iEventNum = 24;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 25;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 26;
	event->lEndKey.push_back(20);
	event->lEndKey.push_back(22);
	event->lEndKey.push_back(23);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 28;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 32;
	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 27;
	event->lEndKey.push_back(21);
	event->lEndKey.push_back(32);
	event->lEndKey.push_back(28);

	event->m_bIsCheckUpdate = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 29;
	event->m_bIsCanReset = true;
	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 30;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 31;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 33;
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
