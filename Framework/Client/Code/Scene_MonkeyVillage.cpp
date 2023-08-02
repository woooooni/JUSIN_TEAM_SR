#include "..\Header\Scene_MonkeyVillage.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_VillagerMonkey.h"
#include "Npc_Dancer.h"
#include "Npc_Elite.h"
#include "Npc_Chief.h"
#include "Npc_Orangi.h"
#include "Npc_Dancer.h"
#include "Npc_DanceTeacher.h"
#include "Portal.h"
#include "UIMgr.h"
#include "BalpanObj.h"
#include "BlockObj.h"
#include "JellyStone.h"
#include "LightFlower.h"
#include "RollingBug.h"
#include "DrawingEnter.h"
#include "DefaultItem.h"
#include "UI_MapName.h"
#include "CutSceneMgr.h"
#include "SkyBox.h" 

static _bool bPlayedCutScene = false;

CScene_MonkeyVillage::CScene_MonkeyVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MONKEY_VILLAGE)
{
}

CScene_MonkeyVillage::~CScene_MonkeyVillage()
{
}

HRESULT CScene_MonkeyVillage::Ready_Scene()
{
	
	__super::Ready_AllLayer();
	Clear_Event();

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
	Play_BGM(L"BGM_7_JungleTown.wav", 0.3f);

	D3DLIGHT9 tLight;
	tLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tLight.Direction = { 0.0f, -1.0f, 1.0f };
	tLight.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	tLight.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	tLight.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	CLightMgr::GetInstance()->Reset_Light();
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Set_LightInfo(tLight);

	return S_OK;
}

_int CScene_MonkeyVillage::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MonkeyVillage::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_MonkeyVillage::Render_Scene()
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

void CScene_MonkeyVillage::Enter_Scene()
{
	if (!bPlayedCutScene)
	{
		CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MONKEY_VILLAGE_INTRO);
		bPlayedCutScene = true;
	}

}

HRESULT CScene_MonkeyVillage::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	
	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 149.f;
	vStartPos.z = 9.f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);
	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	pCamera->Get_TransformCom()->Set_Pos(&_vec3(149.f, 10.f, 9.f));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	CSkyBox* pSkyBox = CSkyBox::Create(m_pGraphicDev, 1);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"Skybox", pSkyBox);
	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Environment()
{
	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST3);
	_vec3 vPortalPos = _vec3(225.f, 1.f, 222.f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
	dynamic_cast<CBoxCollider*>(pPortal->Get_ColliderCom())->Set_Scale(_vec3(10.f, 5.f, 3.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);


	pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MOON_FOREST1);
	vPortalPos = _vec3(141.4f, 70.f, 114.85f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
	dynamic_cast<CBoxCollider*>(pPortal->Get_ColliderCom())->Set_Scale(_vec3(1000.f, 5.f, 1000.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextMoonPortal", pPortal);

	
	CNpc_Chief* pChief = CNpc_Chief::Create(m_pGraphicDev);
	_vec3 vChiefPos = _vec3(141.4f, 0.5f, 114.85f);
	pChief->Get_TransformCom()->Set_Info(INFO_POS, &vChiefPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Chief", pChief);

	CNpc_Elite* pElite = CNpc_Elite::Create(m_pGraphicDev);
	_vec3 vElitePos = _vec3(137.0f, 1.3f, 120.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite1", pElite);

	 pElite = CNpc_Elite::Create(m_pGraphicDev);
	 vElitePos = _vec3(144.5f, 1.3f, 120.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite2", pElite);

	pElite = CNpc_Elite::Create(m_pGraphicDev);
	vElitePos = _vec3(144.5f, 1.3f, 120.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite3", pElite);

	pElite = CNpc_Elite::Create(m_pGraphicDev);
	vElitePos = _vec3(95.5f, 1.3f, 82.5f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite4", pElite);

	pElite = CNpc_Elite::Create(m_pGraphicDev);
	vElitePos = _vec3(98.f, 1.3f, 87.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite5", pElite);

	pElite = CNpc_Elite::Create(m_pGraphicDev);
	vElitePos = _vec3(143.5f, 1.3f, 106.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite6", pElite);

	pElite = CNpc_Elite::Create(m_pGraphicDev);
	vElitePos = _vec3(139.f, 1.3f, 106.f);
	pElite->Get_TransformCom()->Set_Info(INFO_POS, &vElitePos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Elite7", pElite);
	VILLAGERTYPE VillagerTypeArray[5] = { VILLAGERTYPE::MONKEY_HOOD,VILLAGERTYPE::MONKEY_FUR,VILLAGERTYPE::MONKEY_LEAVES1,VILLAGERTYPE::MONKEY_LEAVES2,VILLAGERTYPE::MONKEY_DANCER};
	CNpc_VillagerMonkey* pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand()%5]);
	_vec3 vVillagerMonkeyPos = _vec3(123.f, 0.75f, 22.1f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey_Dancer", pVillagerMonkey);
	
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(122.f, 0.75f, 33.f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(121.f, 0.75f, 45.5f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(112.f, 0.75f, 60.5f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(107.f, 0.75f, 53.f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(102.f, 0.75f, 37.f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(110.f, 0.75f, 20.5f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);
	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(112.5f, 0.75f, 35.3f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(54.f, 0.75f, 107.f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
	vVillagerMonkeyPos = _vec3(82.f, 0.75f, 84.5f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand()%5]);
	vVillagerMonkeyPos = _vec3(80.5f , 0.75f, 108.4f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand()%5]);
	vVillagerMonkeyPos = _vec3(83.5f, 0.75f, 108.4f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand()%5]);
	vVillagerMonkeyPos = _vec3(86.5f, 0.75f, 108.4f);
	pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &vVillagerMonkeyPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);


	//?
	CNpc_DanceTeacher* pDanceTeacher = CNpc_DanceTeacher::Create(m_pGraphicDev);
	_vec3 vDanceTeacherPos = _vec3(141.f, 1.25f, 70.f);
	pDanceTeacher->Get_TransformCom()->Set_Info(INFO_POS, &vDanceTeacherPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DanceTeacher", pDanceTeacher);
	
	CNpc_Dancer* pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	_vec3 vDancerPos = _vec3(144.f, 1.25f, 70.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer1", pDancer);

	pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	vDancerPos = _vec3(138.f, 1.25f, 70.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer2", pDancer);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	vDancerPos = _vec3(159.f, 1.25f, 84.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer3", pDancer);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	vDancerPos = _vec3(161.f, 1.25f, 88.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer4", pDancer);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	vDancerPos = _vec3(163.f, 1.25f, 84.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer5", pDancer);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	pDancer = CNpc_Dancer::Create(m_pGraphicDev);
	vDancerPos = _vec3(161.f, 1.25f, 80.f);
	pDancer->Get_TransformCom()->Set_Info(INFO_POS, &vDancerPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Dancer6", pDancer);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	_vec3 m_vVillagerPos[10] = {_vec3(123.f,0.75f,41.f),_vec3(121.f,0.75f,55.f),
	_vec3(114.f,0.75f,52.f),_vec3(108.f,0.75f,42.f),_vec3(110.f,0.75f,20.f),
	_vec3(115.f,0.75f,31.f),_vec3(127.f,0.75f,30.5f),_vec3(123.f,0.75f,41.f),
	_vec3(105.f,0.75f,41.4f),_vec3(127.f,0.75f,67.5f) };

	for (int i = 0; i < 10; i++)
	{
		pVillagerMonkey = CNpc_VillagerMonkey::Create(m_pGraphicDev, VillagerTypeArray[rand() % 5]);
		
		pVillagerMonkey->Get_TransformCom()->Set_Info(INFO_POS, &m_vVillagerPos[i]);
		m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"VillagerMonkey", pVillagerMonkey);

	}
	CNpc_Orangi* pOrangi = CNpc_Orangi::Create(m_pGraphicDev);
	_vec3 vOrangiPos = _vec3(59.f, 0.75f, 85.49f);
	pOrangi->Get_TransformCom()->Set_Info(INFO_POS, &vOrangiPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Orangi", pOrangi);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	CGameObject* pDrawing = CDrawingEnter::Create(m_pGraphicDev, CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_MONKEYTOWN));
	pDrawing->Get_TransformCom()->Set_Pos(&_vec3(139.5f, 0.6f, 81.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DrawingEnter", pDrawing);

	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Monster()
{
	//CRollingBug* pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(207.f, 0.5f, 151.f), BUGCOLORTYPE::YELLOW);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(207.f, 0.5f, 159.f), BUGCOLORTYPE::PINK);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(215.f, 0.5f, 158.f), BUGCOLORTYPE::BLUE);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(214.f, 0.5f, 149.f), BUGCOLORTYPE::PINK);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(218.f, 0.5f, 146.f), BUGCOLORTYPE::YELLOW);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(224.f, 0.5f, 148.f), BUGCOLORTYPE::BLUE);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	//pRolling = CRollingBug::Create(m_pGraphicDev, _vec3(223.f, 0.5f, 154.f), BUGCOLORTYPE::PINK);
	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"RollingBug", pRolling);

	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_InterationObj()
{
	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_VILLAGE);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();
	return S_OK;
}

HRESULT CScene_MonkeyVillage::Ready_Event()
{

	EVENT* event = new EVENT;
	event->iEventNum = 23;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 24;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 25;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 26;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 27;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 28;
	event->lEndKey.push_back(25);
	event->lEndKey.push_back(26);
	event->lEndKey.push_back(27);

	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));



	return S_OK;
}

CScene_MonkeyVillage* CScene_MonkeyVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MonkeyVillage* pInstance = new CScene_MonkeyVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_MonkeyVillage::Free()
{
	__super::Free();
}
