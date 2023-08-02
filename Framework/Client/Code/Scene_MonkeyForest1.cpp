#include "..\Header\Scene_MonkeyForest1.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Portal.h"
#include "UIMgr.h"
#include "HitObj.h"
#include "BlockObj.h"
#include "Door.h"
#include "DefaultItem.h"
#include "Npc_Artist.h"
#include	"RabbitMgr.h"
#include "DrawingEnter.h"
#include "DoorEnter.h"
#include "UI_MapName.h"


CScene_MonkeyForest1::CScene_MonkeyForest1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MONKEY_FOREST1)
{
}

CScene_MonkeyForest1::~CScene_MonkeyForest1()
{
}

HRESULT CScene_MonkeyForest1::Ready_Scene()
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
	Play_BGM(L"BGM_1_CentralArea.wav", 0.3f);


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

_int CScene_MonkeyForest1::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	//CRabbitMgr::GetInstance()->Update_Object(fTimeDelta);


	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MonkeyForest1::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_MonkeyForest1::Render_Scene()
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

HRESULT CScene_MonkeyForest1::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	
	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Event()
{
	EVENT* event = new EVENT;

	event->iEventNum = 1;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 2;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;

	event->iEventNum = 3;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 4;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 5;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 29;
	FAILED_CHECK(Add_Event(event));


	return S_OK;
}



HRESULT CScene_MonkeyForest1::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 13.15f;
	vStartPos.z = 12.5f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);

	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_Environment()
{
	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_VILLAGE);
	_vec3 vPortalPos = _vec3(74.5f, 0.5f, 83.0f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);

	pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::TUTORIAL_VILLAGE);
	vPortalPos = _vec3(8.f, 0.5f, 13.33f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);
	pPortal->Get_TransformCom()->Set_Scale(_vec3(1.f, 2.f, 20.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"PrevPortal", pPortal);


	CNpc_Artist* pArtist = CNpc_Artist::Create(m_pGraphicDev);
	_vec3 vArtistPos = _vec3(39.5f, 1.25f, 28.91f);
	pArtist->Get_TransformCom()->Set_Info(INFO_POS, &vArtistPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Artist", pArtist);

	CGameObject* pDrawing = CDrawingEnter::Create(m_pGraphicDev, CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_CROSSROAD));
	pDrawing->Get_TransformCom()->Set_Pos(&_vec3(37.5f, 0.6f, 28.91f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DrawingEnter", pDrawing);


	CGameObject* pDoor = CDoor::Create(m_pGraphicDev);
	dynamic_cast<CDoor*>(pDoor)->Set_Door(_vec3(74.5f, 3.0f, 81.5f), _vec3(9.0f, 9.0f, 1.5f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Door", pDoor);

	CGameObject* pEnter = CDoorEnter::Create(m_pGraphicDev);
	pEnter->Get_TransformCom()->Set_Pos(&_vec3(74.5f, 0.5f, 75.5f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DoorEnter", pEnter);

	CDefaultItem* pDefaultItem = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HAT_DRILL);
	NULL_CHECK_RETURN(pDefaultItem, E_FAIL);
	pDefaultItem->Get_TransformCom()->Set_Pos(&_vec3(43.f, 0.5f, 73.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Drill Hat", pDefaultItem);

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	


	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_InterationObj()
{
	CHitObj* pHit = CHitObj::Create(m_pGraphicDev, 1, { 76.f, 0.f, 53.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 2, { 73.5f, 0.f, 56.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 3, { 79.5f, 0.f, 57.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 4, { 71.5f, 0.f, 62.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 5, { 79.5f, 0.f, 65.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 29, { 54.f, 0.f, 35.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_ONCE);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);


	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 69.5f, 0.f, 54.f }, true);
	
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 70.5f, 0.f, 54.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 69.5f, 0.f, 61.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 69.5f, 0.f, 62.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 70.5f, 0.f, 62.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 70.5f, 0.f, 61.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 71.5f, 0.f, 63.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 72.5f, 0.f, 63.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 71.5f, 0.f, 64.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 72.5f, 0.f, 64.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 73.5f, 0.f, 67.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 73.5f, 0.f, 68.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 74.5f, 0.f, 67.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 2, { 74.5f, 0.f, 68.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 75.5f, 0.f, 55.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 76.5f, 0.f, 55.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 75.5f, 0.f, 56.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 76.5f, 0.f, 56.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 75.5f, 0.f, 63.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 76.5f, 0.f, 63.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 75.5f, 0.f, 64.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 3, { 76.5f, 0.f, 64.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 75.5f, 0.f, 57.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 75.5f, 0.f, 58.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);


	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 76.5f, 0.f, 57.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 29, { 53.5f, 0.f, 26.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 29, { 54.5f, 0.f, 26.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);



	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 76.5f, 0.f, 58.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 71.5f, 0.f, 57.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 72.5f, 0.f, 57.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 71.5f, 0.f, 58.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 72.5f, 0.f, 58.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 73.5f, 0.f, 59.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 74.5f, 0.f, 59.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 73.5f, 0.f, 60.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 74.5f, 0.f, 60.f }, false);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 75.5f, 0.f, 67.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 76.5f, 0.f, 67.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 75.5f, 0.f, 68.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 5, { 76.5f, 0.f, 68.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	//CRabbitMgr::GetInstance()->Ready_Object(m_pGraphicDev, 5, 4, { 30, 0, 15 });




	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest1::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST1);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();
	return S_OK;
}

CScene_MonkeyForest1* CScene_MonkeyForest1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MonkeyForest1* pInstance = new CScene_MonkeyForest1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_MonkeyForest1::Free()
{
	__super::Free();
}
