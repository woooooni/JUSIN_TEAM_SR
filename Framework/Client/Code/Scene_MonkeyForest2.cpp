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
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	return S_OK;
}

_int CScene_MonkeyForest2::Update_Scene(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::R))
	{
		Reset(0);
	}

	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);

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
}

HRESULT CScene_MonkeyForest2::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	CUIMgr::GetInstance()->Ready_UIMgr(m_pGraphicDev);

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
	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_VILLAGE);
	_vec3 vPortalPos = _vec3(102.f, 0.5f, 160.f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

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

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Statue", pHit);

	pHit = CHitObj::Create(m_pGraphicDev, 18, { 184.f , 0.f, 36.f });

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

	pBlock = CBlockObj::Create(m_pGraphicDev, 19, { 103.f, 0.f, 43.5f }, true);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 19, { 104.f, 0.f, 43.5f }, true);

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

	Engine::Add_Reset(0, 6, 19);


	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest2::Ready_Layer_UI()
{
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
