#include "..\Header\Scene_MonkeyForest2.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "Portal.h"
#include "UIMgr.h"
#include "HitObj.h"
#include "HoleObj.h"
#include "BlockObj.h"
#include "BalpanObj.h"
#include "PushStone.h"
#include "BreakStone.h"
#include "JellyStone.h"
#include "DrawingEnter.h"
#include "DefaultItem.h"
#include "CutSceneMgr.h"
#include "TriggerObj.h"
#include "RabbitMgr.h"
#include "SkyBox.h"

CScene_MonkeyForest2::CScene_MonkeyForest2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MONKEY_FOREST2)
{
}

CScene_MonkeyForest2::~CScene_MonkeyForest2()
{
}

HRESULT CScene_MonkeyForest2::Ready_Scene()
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
	Play_BGM(L"BGM_6_MiniDungeons.wav", 0.5f);

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

_int CScene_MonkeyForest2::Update_Scene(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::R))
	{
		Reset(0);
	}

	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	CRabbitMgr::GetInstance()->Update_Object(fTimeDelta);

	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MonkeyForest2::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_MonkeyForest2::Render_Scene()
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

HRESULT CScene_MonkeyForest2::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);

	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 102.5f;
	vStartPos.z = 15.f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);
	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();
	CSkyBox* pSkyBox = CSkyBox::Create(m_pGraphicDev, 3);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"Skybox", pSkyBox);
	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Environment()
{
	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::SUNGOLEM_CAVE1);
	_vec3 vPortalPos = _vec3(102.f, 0.5f, 160.f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	CGameObject* pDrawing = CDrawingEnter::Create(m_pGraphicDev, CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_COLORS));
	pDrawing->Get_TransformCom()->Set_Pos(&_vec3(169.5f, 0.6f, 31.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DrawingEnter", pDrawing);


	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_InterationObj()
{
	CHoleObj* pHole = CHoleObj::Create(m_pGraphicDev, 0, { 51.f, 0, 7.5f });

	Add_Subscribe(6, pHole);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Hole", pHole);

	pHole = CHoleObj::Create(m_pGraphicDev, 0, {46.5f, 0, 3.f });
	Add_Subscribe(6, pHole);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Hole", pHole);

	pHole = CHoleObj::Create(m_pGraphicDev, 0, { 26.5f, 0, 26.5f });
	Add_Subscribe(6, pHole);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Hole", pHole);

	CPushStone* pPush = CPushStone::Create({ 26.f, 0.f, 12.f }, m_pGraphicDev);

	Add_Subscribe(6, pPush);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Stone_Push", pPush);

	pPush = CPushStone::Create({ 30.f, 0.f, 14.5f }, m_pGraphicDev);

	Add_Subscribe(6, pPush);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Stone_Push", pPush);

	pPush = CPushStone::Create({ 46.f, 0.f, 26.5f }, m_pGraphicDev);

	Add_Subscribe(6, pPush);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Stone_Push", pPush);

	CHitObj* pHit = CHitObj::Create(m_pGraphicDev, 6, { 49.5f , 0.f, 4.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 7, { 33.f , 0.f, 3.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 8, { 28.f , 0.f, 4.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 9, { 26.5f , 0.f, 28.5f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 17, { 49.f , 0.f, 35.f });

	pHit->Set_CutSceneType(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 35, { 95.f , 0.f, 131.f });

	pHit->Set_HitType(OBJ_HITTYPE::HIT_REPEAT);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);



	
	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 45.f, 0.f, 8.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 45.f, 0.f, 9.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 45.f, 0.f, 10.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 45.f, 0.f, 11.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 25.f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 25.f, 0.f, 8.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 25.f, 0.f, 9.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 25.f, 0.f, 10.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 25.f, 0.f, 11.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 26.f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 26.f, 0.f, 8.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 26.f, 0.f, 9.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 26.f, 0.f, 10.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 26.f, 0.f, 11.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 27.f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 27.f, 0.f, 8.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 27.f, 0.f, 9.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 27.f, 0.f, 10.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 27.f, 0.f, 11.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 28.f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 28.f, 0.f, 8.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 28.f, 0.f, 9.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 28.f, 0.f, 10.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 28.f, 0.f, 11.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 29.f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 29.f, 0.f, 8.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 29.f, 0.f, 9.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 29.f, 0.f, 10.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 6, { 29.f, 0.f, 11.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 32.5f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 33.5f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 34.5f, 0.f, 7.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 25.f, 0.f, 6.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 26.f, 0.f, 6.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 27.f, 0.f, 6.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 28.f, 0.f, 6.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 7, { 29.f, 0.f, 6.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);


	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 25.f, 0.f, 14.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 26.f, 0.f, 14.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 27.f, 0.f, 14.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 28.f, 0.f, 14.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 29.f, 0.f, 14.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 23.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 24.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 25.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 26.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 27.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 8, { 39.5f, 0.f, 28.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 34.f, 0.f, 32.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 35.f, 0.f, 32.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 36.f, 0.f, 32.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 37.f, 0.f, 32.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 38.f, 0.f, 32.5f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 44.f, 0.f, 30.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 9, { 45.f, 0.f, 30.f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 32, { 103.f, 0.f, 43.5f }, true);
	pBlock->Set_BlurEvent(17, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 32, { 104.f, 0.f, 43.5f }, true);
	pBlock->Set_BlurEvent(19, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block_CutScene", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 32, { 105.f, 0.f, 43.5f }, true);
	pBlock->Set_BlurEvent(18, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 33, { 102.f, 0.f, 116.f });
	pBlock->Set_BlurEvent(33, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 34, { 102.f, 0.f, 146.f }, true);
	pBlock->Set_BlurEvent(34, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);




	CBalpanObj* pBalpan = CBalpanObj::Create(m_pGraphicDev, 10, { 49.f, 0.f, 17.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 11, { 50.f, 0.f, 17.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 12, { 51.f, 0.f, 17.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 14, { 44.5f, 0.f, 25.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 15, { 44.5f, 0.f, 28.f });

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 30, { 164.5f, 0.f, 19.5f });
	pBalpan->Set_Static();
	pBalpan->Set_TargName(L"Jelly");
	pBalpan->Set_Answer(JELLY_COLOR::BLUE);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);

	pBalpan = CBalpanObj::Create(m_pGraphicDev, 31, { 174.5f, 0.f, 19.5f });
	pBalpan->Set_Static();
	pBalpan->Set_TargName(L"Jelly");
	pBalpan->Set_Answer(JELLY_COLOR::GREEN);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Balpan", pBalpan);



	CBreakStone* pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 13, { 49.f, 0.f, 16.f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 13, { 50.f, 0.f, 16.f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone); 
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 13, { 51.f, 0.f, 16.f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);


	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 23.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 24.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 25.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 26.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 27.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);
	pBStone = CBreakStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 16, { 47.5f, 0.f, 28.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"BreakStone", pBStone);


	CJellyStone* pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 30, { 166.5f, 0.f, 12.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 30, { 166.5f, 0.f, 10.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 30, { 169.5f, 0.f, 12.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 30, { 169.5f, 0.f, 10.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 30, { 172.5f, 0.f, 10.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);

	pJel = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 30, { 172.5f, 0.f, 12.5f });
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Jelly", pJel);


	Engine::Add_Reset(0, 6, 17);
	Engine::Add_Reset(1, 30, 31);

	CRabbitMgr::GetInstance()->Ready_Object(m_pGraphicDev, 4, 4, { 99, 0, 131 });

	CTriggerObj* pTrig = CTriggerObj::Create(m_pGraphicDev);
	pTrig->Set_EventTrigger(18, []()
		{
			CCutSceneMgr::GetInstance()->Set_EventNum(33);
			CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE);
		});
	pTrig->Set_Once();
	pTrig->Set_Target(CGameMgr::GetInstance()->Get_Player());
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);

	pTrig = CTriggerObj::Create(m_pGraphicDev);
	pTrig->Set_EventTrigger(19, []()
		{
			CCutSceneMgr::GetInstance()->Set_EventNum(32);
			CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_TWO);
		});
	pTrig->Set_Once();
	pTrig->Set_Target(CGameMgr::GetInstance()->Get_Player());

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);

	pTrig = CTriggerObj::Create(m_pGraphicDev);
	pTrig->Get_TransformCom()->Set_Pos(&_vec3(102.f, 0.f, 125.f));
	pTrig->Set_Scale({ 2.8f, 4.f, 3.f });
	pTrig->Add_Trigger([]()
		{
			Check_Event_Start(33);
		}, CTriggerObj::COLLIDE_EVENT_TYPE::ENTER);
	pTrig->Set_Once();
	pTrig->Set_Target(CGameMgr::GetInstance()->Get_Player());

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);

	pTrig = CTriggerObj::Create(m_pGraphicDev);
	pTrig->Set_EventTrigger(35, []()
		{
			if (!CRabbitMgr::GetInstance()->Get_Playing())
			{
				CRabbitMgr::GetInstance()->Start_Game();
			}
		});

	pTrig->Set_Scale({ 0.01f, 0.01f, 0.01f });
	pTrig->Set_Target(CGameMgr::GetInstance()->Get_Player());

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTrig);




	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST2);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Event()
{
	EVENT* event = new EVENT;
	event->iEventNum = 6;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 7;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 8;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 9;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 10;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 11;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 12;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 13;
	event->lEndKey.push_back(10);
	event->lEndKey.push_back(11);
	event->lEndKey.push_back(12);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 14;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 15;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 16;

	event->lEndKey.push_back(14);
	event->lEndKey.push_back(15);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 17;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 18;
	event->lEndKey.push_back(30);
	event->lEndKey.push_back(31);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 19;

	event->lEndKey.push_back(17);
	event->lEndKey.push_back(33);

	event->m_bIsCheckUpdate = true;


	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 30;
	event->m_bIsCanReset = true;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 31;
	event->m_bIsCanReset = true;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 32;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 33;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 34;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 35;
	event->m_bIsCanReset = true;
	FAILED_CHECK(Add_Event(event));



	return S_OK;
}

CScene_MonkeyForest2* CScene_MonkeyForest2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MonkeyForest2* pInstance = new CScene_MonkeyForest2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_MonkeyForest2::Free()
{
	__super::Free();
}
