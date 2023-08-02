#include "Scene_MonkeyForest3.h"

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
#include "FloorDoor.h"
#include "Nexus.h"
#include "DefenceEnter.h"
#include "MonsterSpawner.h"
#include	"Turret.h"

CScene_MonkeyForest3::CScene_MonkeyForest3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::MONKEY_FOREST3)
{
}

CScene_MonkeyForest3::~CScene_MonkeyForest3()
{
}

HRESULT CScene_MonkeyForest3::Ready_Scene()
{
	__super::Ready_AllLayer();

	Clear_Event();

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
	Play_BGM(L"BGM_8_JungleDungeon.wav", 0.5f);

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

_int CScene_MonkeyForest3::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);

	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_MonkeyForest3::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_MonkeyForest3::Render_Scene()
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

void CScene_MonkeyForest3::Enter_Scene()
{
}

HRESULT CScene_MonkeyForest3::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);

	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 63.6f;
	vStartPos.z = 10.4f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	pCamera->Get_TransformCom()->Set_Pos(&_vec3(63.6f, 10.0f, 0.0f));
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);
	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Environment()
{
	/*CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::SUNGOLEM_CAVE1);
	_vec3 vPortalPos = _vec3(102.f, 0.5f, 160.f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();*/


	CGameObject* pLeftDoor = CFloorDoor::Create(m_pGraphicDev);
	dynamic_cast<CFloorDoor*>(pLeftDoor)->Set_Door(_vec3(61.3f, 0.00f, 53.9f), _vec3(5.0f, 10.0f, 0.0f), _vec3(-1.0f, 0.0f, 0.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"LeftDoor", pLeftDoor);

	CGameObject* pRightDoor = CFloorDoor::Create(m_pGraphicDev);
	dynamic_cast<CFloorDoor*>(pRightDoor)->Set_Door(_vec3(66.25f, 0.00f, 53.9f), _vec3(5.0f, 10.0f, 0.0f), _vec3(1.0f, 0.0f, 0.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"RightDoor", pRightDoor);


	CGameObject* pNexus = CNexus::Create(m_pGraphicDev);
	pNexus->Get_TransformCom()->Set_Pos(&_vec3(63.775f, 2.6f, 53.9f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Nexus", pNexus);

	CGameObject* pDefenceEnter = CDefenceEnter::Create(m_pGraphicDev);
	pDefenceEnter->Get_TransformCom()->Set_Pos(&_vec3(64.0f, 2.6f, 16.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"DefenceEnter", pDefenceEnter);

	
	CGameObject* pSpawner = CMonsterSpawner::Create(m_pGraphicDev);
	pSpawner->Get_TransformCom()->Set_Pos(&_vec3(63.0f, 2.6f, 97.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MonsterSpawner", pSpawner);
	dynamic_cast<CMonsterSpawner*>(pSpawner)->Set_Nexus(pNexus);
	dynamic_cast<CNexus*>(pNexus)->Add_Spawner(pSpawner);

	pSpawner = CMonsterSpawner::Create(m_pGraphicDev);
	pSpawner->Get_TransformCom()->Set_Pos(&_vec3(32.0f, 2.6f, 91.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MonsterSpawner", pSpawner);
	dynamic_cast<CMonsterSpawner*>(pSpawner)->Set_Nexus(pNexus);
	dynamic_cast<CNexus*>(pNexus)->Add_Spawner(pSpawner);

	pSpawner = CMonsterSpawner::Create(m_pGraphicDev);
	pSpawner->Get_TransformCom()->Set_Pos(&_vec3(15.0f, 2.6f, 50.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MonsterSpawner", pSpawner);
	dynamic_cast<CMonsterSpawner*>(pSpawner)->Set_Nexus(pNexus);
	dynamic_cast<CNexus*>(pNexus)->Add_Spawner(pSpawner);

	pSpawner = CMonsterSpawner::Create(m_pGraphicDev);
	pSpawner->Get_TransformCom()->Set_Pos(&_vec3(96.0f, 2.6f, 96.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MonsterSpawner", pSpawner);
	dynamic_cast<CMonsterSpawner*>(pSpawner)->Set_Nexus(pNexus);
	dynamic_cast<CNexus*>(pNexus)->Add_Spawner(pSpawner);

	pSpawner = CMonsterSpawner::Create(m_pGraphicDev);
	pSpawner->Get_TransformCom()->Set_Pos(&_vec3(114.0f, 2.6f, 51.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MonsterSpawner", pSpawner);
	dynamic_cast<CMonsterSpawner*>(pSpawner)->Set_Nexus(pNexus);
	dynamic_cast<CNexus*>(pNexus)->Add_Spawner(pSpawner);


	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_InterationObj()
{
	CTurret* turret = CTurret::Create(m_pGraphicDev, 0, { 60.f, 0.f, 49.f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);

 turret = CTurret::Create(m_pGraphicDev, 0, { 67.f, 0.f, 49.f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);

turret = CTurret::Create(m_pGraphicDev, 0, { 58.f, 0.f, 53.5f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);

 turret = CTurret::Create(m_pGraphicDev, 0, { 69.5f, 0.f, 53.5f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);

 turret = CTurret::Create(m_pGraphicDev, 0, { 60.5f, 0.f, 58.5f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);

	turret = CTurret::Create(m_pGraphicDev, 0, { 66.5f, 0.f, 58.5f });

	NULL_CHECK_RETURN(turret, E_FAIL);

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Turret", turret);


	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_MonkeyForest3::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST3);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();
	return S_OK;
}

CScene_MonkeyForest3* CScene_MonkeyForest3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MonkeyForest3* pInstance = new CScene_MonkeyForest3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MonkeyForest3 Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_MonkeyForest3::Free()
{
	__super::Free();
}
