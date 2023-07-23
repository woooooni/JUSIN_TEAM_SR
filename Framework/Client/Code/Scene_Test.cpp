#include "..\Header\Scene_Test.h"
#include "Export_Function.h"
#include "CUI.h"
#include "../Include/stdafx.h"
#include "SkyBox.h"
#include	"JellyBombCreator.h"
#include	"Pool.h"
#include "Player.h"
#include "Monster.h"
#include "RedBeatle.h"
#include "BlueBeatle.h"
#include "GreenBeatle.h"
#include "TrashBig.h"
#include "TrashBummer.h"
#include "DesertRhino.h"
#include "SunGollem.h"
#include "Terrain.h"
#include "PushStone.h"
#include "NearReactObj.h"
#include "NearReactMoonObj.h"
#include "BalpanObj.h"
#include "HitObj.h"
#include "BlockObj.h"
#include "GameMgr.h"
#include "JellyStone.h"
#include "JellyCombined.h"
#include "Catapult.h"
#include "LightPuzzleTerrain.h"

#include "Coin.h"
#include "QuickSlot.h"
#include "NPCText.h"
#include "Icon.h"

#include "UI_Shop.h"
#include "UI_ItemInfo.h"
#include "UI_Cursor.h"
#include "UI_HPBar.h"
#include "UI_BossHP.h"
#include "UI_Totem.h"
#include "UI_ShortCutKey.h"
#include "UI_MonsterHP.h"
#include "LightPuzzleTerrain.h"
#include "LightPuzzleBase.h"
#include "LightPuzzlePiece.h"
#include	"LightFlower.h"
#include	"Turret.h"
#include	"ButterFly.h"
#include	"JellyBomb.h"
#include	"JellyBombCreator.h"
#include	"Grass.h"
#include	"NearReactMushroom.h"

#include "Player_Skill_Range.h"
#include "Player_Skill_Aim.h"

#include "Cupa.h"
#include "TrashSlime.h"
#include "TrashFast.h"
#include "Item_Hat_Monkey.h"

#include "SpitCactus.h"
#include "MothMage.h"
#include "SilkWorm.h"
#include "RollingBug.h"

#include "Item_Hat_Turtle.h"
#include "Item_Hat_Drill.h"
#include "Item_Hat_Light.h"
#include "Item_Hat_Mask.h"
#include "Item_Hat_Missile.h"

#include "ClearBomb.h"
#include "ClearField.h"
#include "PlantCannon.h"
#include "Test_Cube.h"
#include "MonkeyBarrelCleaner.h"

CScene_Test::CScene_Test(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::LOADING)
{
}

CScene_Test::~CScene_Test()
{
}

HRESULT CScene_Test::Ready_Scene()
{
	__super::Ready_AllLayer();
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pGraphicDev->SetViewport(&vp);

	CPool<CJellyStone>::Ready_Pool(m_pGraphicDev, 0);
	CPool<CJellyBomb>::Ready_Pool(m_pGraphicDev, 0);



	return S_OK;
}

Engine::_int CScene_Test::Update_Scene(const _float& fTimeDelta)
{
	Check_Event_Start(10);

	__super::Update_Scene(fTimeDelta);

	return 0;
}

void CScene_Test::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CScene_Test::Render_Scene()
{

}

void CScene_Test::Free()
{
	__super::Free();
}

CScene_Test* CScene_Test::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Test* pInstance = new CScene_Test(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Scene_Test Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CScene_Test::Ready_Prototype()
{
	FAILED_CHECK_RETURN(CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev), E_FAIL);
	return S_OK;
}



HRESULT CScene_Test::Ready_Layer_Player()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer), E_FAIL);
	m_pPlayer = pPlayer;

//	CItem_Hat_Monkey* pItemMonkeyHat = CItem_Hat_Monkey::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemMonkeyHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MonkeyHat", pItemMonkeyHat), E_FAIL);
//
//
//	CItem_Hat_Turtle* pItemTurtleHat = CItem_Hat_Turtle::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemTurtleHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_TurtleHat", pItemTurtleHat), E_FAIL);
//
//
//	CItem_Hat_Drill* pItemDrillHat = CItem_Hat_Drill::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemDrillHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_DrillHat", pItemDrillHat), E_FAIL);
//
//	CItem_Hat_Light* pItemLightHat = CItem_Hat_Light::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemLightHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_LightHat", pItemLightHat), E_FAIL);
//
//	CItem_Hat_Mask* pItemMaskHat = CItem_Hat_Mask::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemMaskHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MaskHat", pItemMaskHat), E_FAIL);
//
//	CItem_Hat_Missile* pItemMissileHat = CItem_Hat_Missile::Create(m_pGraphicDev, pPlayer);
//	NULL_CHECK_RETURN(pItemMissileHat, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MissileHat", pItemMissileHat), E_FAIL);



//	pPlayer->Set_Hat(pItemMaskHat);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_Camera()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::CAMERA];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CLayer* pPlayerLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pPlayerLayer->Find_GameObject(L"Player"));

	// Camera
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pCamera), E_FAIL);

	pCamera->Set_TargetObj(pPlayer);
	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_Terrrain()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::TERRAIN];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_Environment()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::ENVIRONMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	/*CTest_Cube* pCube = CTest_Cube::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCube, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Test_Cube", pCube), E_FAIL);*/
	//CNpc_Cow* pNPCCow = CNpc_Cow::Create(m_pGraphicDev, { 10, 1, 3 }, NPCTYPE::TUT_COW);
	//NULL_CHECK_RETURN(pNPCCow, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Cow", pNPCCow), E_FAIL);

	//CNpcSheep* pNPCSheep = CNpcSheep::Create(m_pGraphicDev, { 18, 1, 7 }, NPCTYPE::TUT_SHEEP);
	//NULL_CHECK_RETURN(pNPCSheep, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Sheep", pNPCSheep), E_FAIL);
	CMonkeyBarrelCleaner* pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
	pMonkeyBarrelCleaner->Set_Right(false);
	pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&_vec3(7.f, 0.5f, 1.f));
	NULL_CHECK_RETURN(pMonkeyBarrelCleaner, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner), E_FAIL);

	pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
	pMonkeyBarrelCleaner->Set_Right(false);
	pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&_vec3(7.f, 0.5f, 3.f));
	NULL_CHECK_RETURN(pMonkeyBarrelCleaner, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner), E_FAIL);

	pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
	pMonkeyBarrelCleaner->Set_Right(true);
	pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&_vec3(-6.f, 0.5f, 1.f));
	NULL_CHECK_RETURN(pMonkeyBarrelCleaner, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner), E_FAIL);

	pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
	pMonkeyBarrelCleaner->Set_Right(true);
	pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&_vec3(-6.f, 0.5f, 2.f));
	NULL_CHECK_RETURN(pMonkeyBarrelCleaner, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner), E_FAIL);

	pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
	pMonkeyBarrelCleaner->Set_Right(true);
	pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&_vec3(-6.f, 0.5f, 3.f));
	NULL_CHECK_RETURN(pMonkeyBarrelCleaner, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

//	CRollingBug* pMonRolling_Pink = CRollingBug::Create(m_pGraphicDev, _vec3(2.f, 0.5f, 3.f), BUGCOLORTYPE::PINK);
//	NULL_CHECK_RETURN(pMonRolling_Pink, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rolling_Pink", pMonRolling_Pink), E_FAIL);
//
//	CRollingBug* pMonRolling_Blue = CRollingBug::Create(m_pGraphicDev, _vec3(6.f, 0.5f, 4.f), BUGCOLORTYPE::BLUE);
//	NULL_CHECK_RETURN(pMonRolling_Blue, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rolling_Blue", pMonRolling_Blue), E_FAIL);
//
//	CRollingBug* pMonRolling_Yellow = CRollingBug::Create(m_pGraphicDev, _vec3(8.f, 0.5f, 2.f), BUGCOLORTYPE::YELLOW);
//	NULL_CHECK_RETURN(pMonRolling_Yellow, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rolling_Yellow", pMonRolling_Yellow), E_FAIL);
	//
	////	CCupa* pMonCupa = CCupa::Create(m_pGraphicDev);
	////	NULL_CHECK_RETURN(pMonCupa, E_FAIL);
	////	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Cupa", pMonCupa), E_FAIL);
	//
	//	/*CBlueBeatle* pBlueBeatle = CBlueBeatle::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pBlueBeatle, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlueBeatle", pBlueBeatle), E_FAIL);
	//
	//	CRedBeatle* pRedBeatle = CRedBeatle::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pRedBeatle, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RedBeatle", pRedBeatle), E_FAIL);
	//
	//	CGreenBeatle* pGreenBeatle = CGreenBeatle::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGreenBeatle, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GreenBeatle", pGreenBeatle), E_FAIL);
	//
//	CTrashBig* pTrashBig = CTrashBig::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pTrashBig, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CTrashBig", pTrashBig), E_FAIL);
//	//
//	CTrashSlime* pTrashSlime = CTrashSlime::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pTrashSlime, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CTrashSlime", pTrashSlime), E_FAIL);
	//
	//	CTrashFast* pTrashFast = CTrashFast::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pTrashFast, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TrashFast", pTrashFast), E_FAIL);
	//
//		CSpitCactus* pSpitCactus = CSpitCactus::Create(m_pGraphicDev);
//		NULL_CHECK_RETURN(pSpitCactus, E_FAIL);
//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpitCactus", pSpitCactus), E_FAIL);
//		CTrashBummer* pTrashBummer = CTrashBummer::Create(m_pGraphicDev);
//		NULL_CHECK_RETURN(pTrashBummer, E_FAIL);
//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TrashBummer", pTrashBummer), E_FAIL);
//
//	CMothMage* pMothMage = CMothMage::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pMothMage, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MothMage", pMothMage), E_FAIL);

	//	CPlantCannon* pPlantCannon = CPlantCannon::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pPlantCannon, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlantCannon", pPlantCannon), E_FAIL);


//	CDesertRhino* pDesertRhino = CDesertRhino::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pDesertRhino, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DesertRhino", pDesertRhino), E_FAIL);
//
//	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SunGollem", pSunGollem), E_FAIL);

	//CDesertRhino* pDesertRhino = CDesertRhino::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pDesertRhino, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DesertRhino", pDesertRhino), E_FAIL);



	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SunGollem", pSunGollem), E_FAIL);

	/*CSilkWorm* pSilkWorm = CSilkWorm::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSilkWorm, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SilkWorm", pSilkWorm), E_FAIL);*/

	pLayer->Ready_Layer();

	return S_OK;
}
HRESULT CScene_Test::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CPushStone* pPush = CPushStone::Create(_vec3(1, 1, 3), m_pGraphicDev);
	NULL_CHECK_RETURN(pPush, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pPush), E_FAIL);


	CNearReactObj* pNear = CNearReactObj::Create(m_pGraphicDev, { 5, 1, 5 });
	NULL_CHECK_RETURN(pNear, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gosari", pNear), E_FAIL);

	pNear = CNearReactObj::Create(m_pGraphicDev, { 10, 1, 10 });
	NULL_CHECK_RETURN(pNear, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gosari", pNear), E_FAIL);


	pNear = CNearReactObj::Create(m_pGraphicDev, { 10, 1, 5 });
	NULL_CHECK_RETURN(pNear, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gosari", pNear), E_FAIL);

	pNear = CNearReactObj::Create(m_pGraphicDev, { 5, 1, 10 });
	NULL_CHECK_RETURN(pNear, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gosari", pNear), E_FAIL);

	CNearReactMoonObj* pMoonNear = CNearReactMoonObj::Create(m_pGraphicDev, { 13, 1, 13 });
	NULL_CHECK_RETURN(pMoonNear, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DalPul", pMoonNear), E_FAIL);

	CNearReactMushroom* pNearMush = CNearReactMushroom::Create(m_pGraphicDev, { 1, 0, 4 });
	NULL_CHECK_RETURN(pNearMush, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonReactMush", pNearMush), E_FAIL);


	CBalpanObj* pBal = CBalpanObj::Create(m_pGraphicDev, 1, { 13, 1, 15 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Set_AutoReset();
	pBal->Set_TargName(L"Stone");
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Balpan", pBal), E_FAIL);

	pBal = CBalpanObj::Create(m_pGraphicDev, 0, { 15, 1, 15 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Add_ActivateState(3);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Balpan", pBal), E_FAIL);

	pBal = CBalpanObj::Create(m_pGraphicDev, 0, { 17, 1, 15 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Add_ActivateState(3);
	pBal->Set_AutoReset();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Balpan", pBal), E_FAIL);

	pBal = CBalpanObj::Create(m_pGraphicDev, 5, { 10, 1, 3 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Static();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Balpan", pBal), E_FAIL);

	pBal = CBalpanObj::Create(m_pGraphicDev, 11, { 19, 1, 20 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Set_TargName(L"Jelly");
	pBal->Set_Static();
	pBal->Set_Answer(JELLY_COLOR::MAGENTA);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Balpan", pBal), E_FAIL);



	CLightFlower* pLF = CLightFlower::Create(m_pGraphicDev, pBal, 0, { 10, 1, 5 });
	NULL_CHECK_RETURN(pLF, E_FAIL);
	pLF->Add_Subscribe(5);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightFlower", pLF), E_FAIL);

	CHitObj* pHit = CHitObj::Create(m_pGraphicDev, 0, { 17, 1, 17 });
	NULL_CHECK_RETURN(pHit, E_FAIL);
	pHit->Set_Event(2);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugDongsang", pHit), E_FAIL);

	


	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 11, { 15, 1, 20 }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlockObj", pBlock), E_FAIL);

	
	CJellyStone* pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 0, { 8, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);

	pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 0, { 5, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);

	pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 0, { 11, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);


	CCatapult* pCata = CCatapult::Create(m_pGraphicDev, 0, { 5, 1 , 20 });
	NULL_CHECK_RETURN(pCata, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult", pCata), E_FAIL);

	CLightPuzzleTerrain* pLTer = CLightPuzzleTerrain::Create(m_pGraphicDev, 4, 5, { 25, 0 , 25 });
	NULL_CHECK_RETURN(pLTer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzleTer", pLTer), E_FAIL);

	CLightPuzzleBase* pLBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(0, 0), L"Base");
	pLBase->Set_MakeLight();
	NULL_CHECK_RETURN(pLBase, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzleBase", pLBase), E_FAIL);

	pLBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(3, 4), L"Vertical");
	NULL_CHECK_RETURN(pLBase, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzleBase", pLBase), E_FAIL);

	CLightPuzzlePiece* pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(3, 3));
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	pLPiece->Reverse_Puzzle(true);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);

	pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(3, 0), L"Corner");
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	pLPiece->Reverse_Puzzle(false);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);

	pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(3, 1), L"Horizon");
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	pLPiece->Reverse_Puzzle(false);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);

	pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(0, 3), L"Vertical");
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	pLPiece->Reverse_Puzzle(false);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);
	pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(1, 1), L"Base_Up");
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);

	pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, pLTer->Get_TilePos(1, 3));
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzlePiece", pLPiece), E_FAIL);

	CTurret* pTurret = CTurret::Create(m_pGraphicDev, 0, { 0, 1, 2 });
	NULL_CHECK_RETURN(pTurret, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Turret", pTurret), E_FAIL);

	CButterFly* pBut = CButterFly::Create(m_pGraphicDev, ITEM_CODE::BUTTERFLY, { 20, 1, 20 });
	NULL_CHECK_RETURN(pBut, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ButterFly", pBut), E_FAIL);

	CJellyBomb* pJelBomb = CJellyBomb::Create(m_pGraphicDev, 6, { 5, 1, 3 });
	NULL_CHECK_RETURN(pJelBomb, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Bomb", pJelBomb), E_FAIL);

	CJellyBombCreator* pJelCreat = CJellyBombCreator::Create(m_pGraphicDev, pJelBomb, 0, { 3, 1, 2 });
	NULL_CHECK_RETURN(pJelCreat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Bomb_Creator", pJelCreat), E_FAIL);

	CGrass* pGrass = CGrass::Create(m_pGraphicDev, GRASS_TYPE::GLOWING_REED_RED, 0, { 3, 0, 1 });
	NULL_CHECK_RETURN(pGrass, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGrass), E_FAIL);

	pGrass = CGrass::Create(m_pGraphicDev, GRASS_TYPE::HEALTHBUSH, 0, { 4.5, 0, 1 });
	NULL_CHECK_RETURN(pGrass, E_FAIL);


	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGrass), E_FAIL);

	CClearBomb* pCBomb = CClearBomb::Create(m_pGraphicDev, { 2, 0, 2 });
	NULL_CHECK_RETURN(pCBomb, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ClearBomb", pCBomb), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_Effect()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::EFFECT];
	Engine::CLayer* pPlayerLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pPlayerLayer->Find_GameObject(L"Player"));

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CPlayer_Skill_Range* pPlayerSkillRange = CPlayer_Skill_Range::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayerSkillRange, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Skill_Range", pPlayerSkillRange), E_FAIL);
	pPlayer->Set_SkillRange(pPlayerSkillRange);

	CPlayer_Skill_Aim* pPlayerSkillAim = CPlayer_Skill_Aim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayerSkillAim, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Skill_Aim", pPlayerSkillAim), E_FAIL);
	pPlayer->Set_Aim(pPlayerSkillAim);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Test::Ready_Layer_UI()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}





