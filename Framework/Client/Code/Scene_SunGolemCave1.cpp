#include "..\Header\Scene_SunGolemCave1.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "Portal.h"
#include "UIMgr.h"
#include "SunGollem.h"
#include	"Catapult.h"
#include "BossDoor.h"
#include "BossDoorEnter.h"
#include "TriggerObj.h"
#include "CutSceneMgr.h"
#include "BlockObj.h"

CScene_SunGolemCave1::CScene_SunGolemCave1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::SUNGOLEM_CAVE1)
{

}

CScene_SunGolemCave1::~CScene_SunGolemCave1()
{
}

HRESULT CScene_SunGolemCave1::Ready_Scene()
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
	Play_BGM(L"BGM_9_SunGolem.wav", 0.5f);


	return S_OK;
}

_int CScene_SunGolemCave1::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_SunGolemCave1::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();
	__super::LateUpdate_Scene();
}

void CScene_SunGolemCave1::Render_Scene()
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

HRESULT CScene_SunGolemCave1::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	
	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 9.0f;
	vStartPos.z = -40.f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);

	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Environment()
{

	CGameObject* pLeftDoor = CBossDoor::Create(m_pGraphicDev);
	dynamic_cast<CBossDoor*>(pLeftDoor)->Set_Door(_vec3(7.4f, 4.7f, 20.5f), _vec3(3.0f, 9.6f, 0.0f), _vec3(-1.0f, 0.0f, 0.0f));
	Add_Subscribe(1, pLeftDoor);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"LeftDoor", pLeftDoor);

	CGameObject* pRightDoor = CBossDoor::Create(m_pGraphicDev);
	dynamic_cast<CBossDoor*>(pRightDoor)->Set_Door(_vec3(10.4f, 4.7f, 20.5f), _vec3(3.0f, 9.6f, 0.0f), _vec3(1.0f, 0.0f, 0.0f));
	Add_Subscribe(1, pRightDoor);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"RightDoor", pRightDoor);

	/*CGameObject* pEnter = CBossDoorEnter::Create(m_pGraphicDev);
	pEnter->Get_TransformCom()->Set_Pos(&_vec3(8.8f, 0.5f, 16.0f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"BossDoorEnter", pEnter);*/


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Monster()
{
	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
	pSunGollem->Set_Stop(true);
	m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"SunGollem", pSunGollem);

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_InterationObj()
{
	CCatapult* pCat = CCatapult::Create(m_pGraphicDev, 0, { 10.f, 0.f, 5.f });
	NULL_CHECK_RETURN(pCat, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Catapult", pCat);

	CTriggerObj* pTri = CTriggerObj::Create(m_pGraphicDev, { 9.f, 0.f, -21.4 });
	pTri->Set_Scale({ 5.f, 5.f, 5.f });
	pTri->Add_Trigger([]()
		{
			CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::BOSS_SUNGOLEM_INTRO);
			CCutSceneMgr::GetInstance()->Set_EventNum(1);
		}, CTriggerObj::COLLIDE_EVENT_TYPE::ENTER);
	pTri->Set_Once();
	pTri->Set_Target(CGameMgr::GetInstance()->Get_Player());
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Trigger", pTri);

	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 1, { -2.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { -1.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { -0.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 0.5f, 0.f, 10.5f }, true);

	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);


	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 1.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 2.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 3.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 4.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);
	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 5.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 6.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 7.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 8.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 9.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 10.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 11.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 12.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 13.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 14.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 15.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 16.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 17.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 18.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 19.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);

	 pBlock = CBlockObj::Create(m_pGraphicDev, 1, { 20.5f, 0.f, 10.5f }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	pBlock->Set_BlurEvent(2, L"Monkey");
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Block", pBlock);


	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::SUNGOLEM_CAVE1);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Event()
{
	EVENT* event = new EVENT;
	event->iEventNum = 1;
	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 2;
	FAILED_CHECK(Add_Event(event));

	return S_OK;
}

CScene_SunGolemCave1* CScene_SunGolemCave1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_SunGolemCave1* pInstance = new CScene_SunGolemCave1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_SunGolemCave1::Free()
{
	__super::Free();
}
