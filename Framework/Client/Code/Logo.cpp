#include "..\Header\Logo.h"
#include "Export_Function.h"
#include "CUI.h"
#include "../Include/stdafx.h"
#include "SkyBox.h"
#include	"JellyBombCreator.h"
#include	"Pool.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::LOADING)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
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
	CPool<CJellyCombined>::Ready_Pool(m_pGraphicDev, 0);
	CPool<CJellyBomb>::Ready_Pool(m_pGraphicDev, 0);

	MATERIAL.Set_Material(MATERIAL.material, { 1.f, 1.f, 1.f, 0.f });

	D3DMATERIAL9 mater = MATERIAL.material;

	FAILED_CHECK(m_pGraphicDev->SetMaterial(&MATERIAL.material));


	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{

	__super::Update_Scene(fTimeDelta);

	return 0;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	
}

void CLogo::Free()
{
	__super::Free();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CLogo::Ready_Prototype()
{
	return S_OK;
}



HRESULT CLogo::Ready_Layer_Player()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::PLAYER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer), E_FAIL);

	CItem_Hat_Monkey* pItemMonkeyHat = CItem_Hat_Monkey::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemMonkeyHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MonkeyHat", pItemMonkeyHat), E_FAIL);


	CItem_Hat_Turtle* pItemTurtleHat = CItem_Hat_Turtle::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemTurtleHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_TurtleHat", pItemTurtleHat), E_FAIL);


	CItem_Hat_Drill* pItemDrillHat = CItem_Hat_Drill::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemDrillHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_DrillHat", pItemDrillHat), E_FAIL);

	CItem_Hat_Light* pItemLightHat = CItem_Hat_Light::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemLightHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_LightHat", pItemLightHat), E_FAIL);


	CItem_Hat_Mask* pItemMaskHat = CItem_Hat_Mask::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemMaskHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MaskHat", pItemMaskHat), E_FAIL);

	CItem_Hat_Missile* pItemMissileHat = CItem_Hat_Missile::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pItemMissileHat, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_MissileHat", pItemMissileHat), E_FAIL);


	pPlayer->Set_Hat(pItemMonkeyHat);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Camera()
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

HRESULT CLogo::Ready_Layer_Terrrain()
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

HRESULT CLogo::Ready_Layer_Environment()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::ENVIRONMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	CCoin* pCoin = CCoin::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCoin, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Coin", pCoin), E_FAIL);

	CNPCCow* pNPCCow = CNPCCow::Create(m_pGraphicDev, { 10, 1, 3 }, NPCTYPE::TUT_COW);
	NULL_CHECK_RETURN(pNPCCow, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Cow", pNPCCow), E_FAIL);

	CNPCSheep* pNPCSheep = CNPCSheep::Create(m_pGraphicDev, { 18, 1, 7 }, NPCTYPE::TUT_SHEEP);
	NULL_CHECK_RETURN(pNPCSheep, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Sheep", pNPCSheep), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

//	CRollingBug* pMonRolling_Pink = CRollingBug::Create(m_pGraphicDev, _vec3(20.f, 1.f, 20.f), BUGCOLORTYPE::PINK);
//	NULL_CHECK_RETURN(pMonRolling_Pink, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rolling_Pink", pMonRolling_Pink), E_FAIL);
//
//	CRollingBug* pMonRolling_Blue = CRollingBug::Create(m_pGraphicDev, _vec3(6.f, 1.f, 4.f), BUGCOLORTYPE::BLUE);
//	NULL_CHECK_RETURN(pMonRolling_Blue, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster_Rolling_Blue", pMonRolling_Blue), E_FAIL);
//
//	CRollingBug* pMonRolling_Yellow = CRollingBug::Create(m_pGraphicDev, _vec3(8.f, 1.f, 2.f), BUGCOLORTYPE::YELLOW);
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
//
//	CTrashSlime* pTrashSlime = CTrashSlime::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pTrashSlime, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CTrashSlime", pTrashSlime), E_FAIL);
//
//	CTrashFast* pTrashFast = CTrashFast::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pTrashFast, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TrashFast", pTrashFast), E_FAIL);
//
//	CSpitCactus* pSpitCactus = CSpitCactus::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pSpitCactus, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpitCactus", pSpitCactus), E_FAIL);
//		*/
//
//	CTrashBummer* pTrashBummer = CTrashBummer::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pTrashBummer, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TrashBummer", pTrashBummer), E_FAIL);
//
//
//	CMothMage* pMothMage = CMothMage::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pMothMage, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpitCactus", pMothMage), E_FAIL);
//	
//	CPlantCannon* pPlantCannon = CPlantCannon::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pPlantCannon, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlantCannon", pPlantCannon), E_FAIL);


	//CDesertRhino* pDesertRhino = CDesertRhino::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pDesertRhino, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DesertRhino", pDesertRhino), E_FAIL);
	//
	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SunGollem", pSunGollem), E_FAIL);

	/*CSilkWorm* pSilkWorm = CSilkWorm::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSilkWorm, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SilkWorm", pSilkWorm), E_FAIL);*/
//	CSilkWorm* pSilkWorm = CSilkWorm::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pSilkWorm, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SilkWorm", pSilkWorm), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CPushStone* pPush = CPushStone::Create(_vec3(3, 1, 3), m_pGraphicDev);
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

	CLightFlower* pLF = CLightFlower::Create(m_pGraphicDev, pBal, 0, { 10, 1, 5 });
	NULL_CHECK_RETURN(pLF, E_FAIL);
	pLF->Add_Subscribe(5);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightFlower", pLF), E_FAIL);

	CHitObj* pHit = CHitObj::Create(m_pGraphicDev, 0, { 17, 1, 17 });
	NULL_CHECK_RETURN(pHit, E_FAIL);
	pHit->Set_Event(2);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugDongsang", pHit), E_FAIL);

	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 15, 1, 20 }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlockObj", pBlock), E_FAIL);

	CJellyStone* pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::CYAN, 0, { 8, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);

	pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::MAGENTA, 0, { 5, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);

	pJelly = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::YELLOW, 0, { 11, 1, 10 });
	NULL_CHECK_RETURN(pJelly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Normal", pJelly), E_FAIL);

	CJellyCombined* pCombine = CJellyCombined::Create(m_pGraphicDev, JELLY_COLLOR_COMBINE::RED, 0, { 5, 1, 13 });
	NULL_CHECK_RETURN(pCombine, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Jelly_Combined", pCombine), E_FAIL);

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
	pGrass->Add_DropItem(ITEM_CODE::TWIG, 80);
	pGrass->Add_DropItem(ITEM_CODE::LEAF, 10);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGrass), E_FAIL);

	pGrass = CGrass::Create(m_pGraphicDev, GRASS_TYPE::HEALTHBUSH, 0, { 4.5, 0, 1 });
	NULL_CHECK_RETURN(pGrass, E_FAIL);
	pGrass->Add_DropItem(ITEM_CODE::HP_SMALL, 50);
	pGrass->Add_DropItem(ITEM_CODE::HP_MIDDLE, 30);
	pGrass->Add_DropItem(ITEM_CODE::HP_BIG, 15);


	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGrass), E_FAIL);

	CEtcItem* pEtc = CEtcItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::TWIG);
	NULL_CHECK_RETURN(pEtc, E_FAIL);
	pEtc->Get_TransformCom()->Set_Pos(&_vec3(1, 0, 2));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item", pEtc), E_FAIL);

	CClearBomb* pCBomb = CClearBomb::Create(m_pGraphicDev, { 2, 0, 2 });
	NULL_CHECK_RETURN(pCBomb, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ClearBomb", pCBomb), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Effect()
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

HRESULT CLogo::Ready_Layer_UI()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	// UI (Boss)
	//CUI_BossHP* pUI_BossBack = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_BACK);
	//NULL_CHECK_RETURN(pUI_BossBack, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_Boss_HPBack", pUI_BossBack), E_FAIL);

	//CUI_BossHP* pUI_BossGauge = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_GAUGE);
	//NULL_CHECK_RETURN(pUI_BossGauge, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_Boss_Gauge", pUI_BossGauge), E_FAIL);

	//CUI_BossHP* pUI_BossFrame = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_FRAME);
	//NULL_CHECK_RETURN(pUI_BossFrame, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_Boss_HPFrame", pUI_BossFrame), E_FAIL);

	//// UI (Monster)
	//CUI_MonsterHP* pUI_MonsterBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	//NULL_CHECK_RETURN(pUI_MonsterBack, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_HPBack", pUI_MonsterBack), E_FAIL);

	//CUI_MonsterHP* pUI_MonsterGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	//NULL_CHECK_RETURN(pUI_MonsterGauge, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_Gauge", pUI_MonsterGauge), E_FAIL);

	//CUI_MonsterHP* pUI_MonsterFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	//NULL_CHECK_RETURN(pUI_MonsterFrame, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Monster_HPFrame", pUI_MonsterFrame), E_FAIL);
	
	// UI (SHOP)
	CUI_Shop* pUI_Shop = CUI_Shop::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI_Shop, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Background", pUI_Shop), E_FAIL);

	CUI_ItemInfo* pUI_Branch = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_BRANCH);
	NULL_CHECK_RETURN(pUI_Branch, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Branch", pUI_Branch), E_FAIL);

	CUI_ItemInfo* pUI_Cloth = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_CLOTH);
	NULL_CHECK_RETURN(pUI_Cloth, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Cloth", pUI_Cloth), E_FAIL);

	CUI_ItemInfo* pUI_Leaf = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_LEAF);
	NULL_CHECK_RETURN(pUI_Leaf, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Leaf", pUI_Leaf), E_FAIL);

	CUI_ItemInfo* pUI_PriceTag = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_PRICETAG);
	NULL_CHECK_RETURN(pUI_PriceTag, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_PriceTag", pUI_PriceTag), E_FAIL);

	CUI_ItemInfo* pShop_TextBox = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_TEXTBOX);
	NULL_CHECK_RETURN(pShop_TextBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_TextBox", pShop_TextBox), E_FAIL);

	CUI_ItemInfo* pWallet = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_WALLET);
	NULL_CHECK_RETURN(pWallet, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Wallet", pWallet), E_FAIL);

	CUI_Cursor* pUI_Cursor = CUI_Cursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI_Cursor, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Cursor", pUI_Cursor), E_FAIL);

	CUI_ItemInfo* pShop_ImgBox = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_IMGBOX);
	NULL_CHECK_RETURN(pShop_ImgBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_IMGBox", pShop_ImgBox), E_FAIL);

	CUI_ItemInfo* pUI_Branch_Info = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_BRANCH_INFO);
	NULL_CHECK_RETURN(pUI_Branch_Info, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Branch_Info", pUI_Branch_Info), E_FAIL);

	CUI_ItemInfo* pUI_Cloth_Info = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_CLOTH_INFO);
	NULL_CHECK_RETURN(pUI_Cloth_Info, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Cloth_Info", pUI_Cloth_Info), E_FAIL);

	CUI_ItemInfo* pUI_Leaf_Info = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::UISHOP_LEAF_INFO);
	NULL_CHECK_RETURN(pUI_Leaf_Info, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_Leaf_Info", pUI_Leaf_Info), E_FAIL);


	// UI

	CNPCTextBox* pTextBox = CNPCTextBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTextBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_TextBox", pTextBox), E_FAIL);

	CNPCText* pText = CNPCText::Create(m_pGraphicDev, TEXTTYPE::COW);
	NULL_CHECK_RETURN(pText, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Text", pText), E_FAIL);

	CIcon* pIconHeart = CIcon::Create(m_pGraphicDev, ICONTYPE::HEART);
	NULL_CHECK_RETURN(pIconHeart, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Heart", pIconHeart), E_FAIL);

	CIcon* pIconKeyboard = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBOARD);
	NULL_CHECK_RETURN(pIconKeyboard, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Keyboard", pIconKeyboard), E_FAIL);

	CIcon* pIconQuest = CIcon::Create(m_pGraphicDev, ICONTYPE::QUEST);
	NULL_CHECK_RETURN(pIconQuest, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Heart", pIconQuest), E_FAIL);
	
	CIcon* pHPBack = CIcon::Create(m_pGraphicDev, ICONTYPE::PLAYERHP_BACK);
	NULL_CHECK_RETURN(pHPBack, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HPBack", pHPBack), E_FAIL);

	CUI_HPBar* pHPBar = CUI_HPBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pHPBar, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HPBar", pHPBar), E_FAIL);

	CIcon* pIconHPFrame = CIcon::Create(m_pGraphicDev, ICONTYPE::PLAYERHP_FRAME);
	NULL_CHECK_RETURN(pIconHPFrame, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_HPFrame", pIconHPFrame), E_FAIL);

//	CUI_Totem* pTotem_Cur = CUI_Totem::Create(m_pGraphicDev, TOTEMTYPE::CURR);
//	NULL_CHECK_RETURN(pTotem_Cur, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Totem_Cur", pTotem_Cur), E_FAIL);
//
//	CUI_Totem* pTotem_Max = CUI_Totem::Create(m_pGraphicDev, TOTEMTYPE::MAX);
//	NULL_CHECK_RETURN(pTotem_Max, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Totem_Max", pTotem_Max), E_FAIL);

	CIcon* pIconKeyButton = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON_L);
	NULL_CHECK_RETURN(pIconKeyButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton_L", pIconKeyButton), E_FAIL);

	CQuickSlot* pQuickSlot1 = CQuickSlot::Create(m_pGraphicDev, SLOTNUM::SLOT_ONE);
	NULL_CHECK_RETURN(pQuickSlot1, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_QuickSlot_First", pQuickSlot1), E_FAIL);

	CQuickSlot* pQuickSlot2 = CQuickSlot::Create(m_pGraphicDev, SLOTNUM::SLOT_TWO);
	NULL_CHECK_RETURN(pQuickSlot2, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_QuickSlot_Second", pQuickSlot2), E_FAIL);

	CQuickSlot* pQuickSlot3 = CQuickSlot::Create(m_pGraphicDev, SLOTNUM::SLOT_THREE);
	NULL_CHECK_RETURN(pQuickSlot3, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_QuickSlot_Third", pQuickSlot3), E_FAIL);

	CQuickSlot* pQuickSlot4 = CQuickSlot::Create(m_pGraphicDev, SLOTNUM::SLOT_FOUR);
	NULL_CHECK_RETURN(pQuickSlot4, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_QuickSlot_Fourth", pQuickSlot4), E_FAIL);

	CIcon* pIconKeyButton1 = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON_1);
	NULL_CHECK_RETURN(pIconKeyButton1, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton_1", pIconKeyButton1), E_FAIL);

	CIcon* pIconKeyButton2 = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON_2);
	NULL_CHECK_RETURN(pIconKeyButton2, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton_2", pIconKeyButton2), E_FAIL);

	CIcon* pIconKeyButton3 = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON_3);
	NULL_CHECK_RETURN(pIconKeyButton3, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton_3", pIconKeyButton3), E_FAIL);

	CIcon* pIconKeyButton4 = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON_4);
	NULL_CHECK_RETURN(pIconKeyButton4, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton_4", pIconKeyButton4), E_FAIL);

	CUI_ShortCutKey* pZKeyInfo = CUI_ShortCutKey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pZKeyInfo, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_ShortCutKey_Info", pZKeyInfo), E_FAIL);

	CUI_ItemInfo* pLKeyInfo = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOPKEY_L);
	NULL_CHECK_RETURN(pLKeyInfo, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_ShortCutKey_Close", pLKeyInfo), E_FAIL);

	CUI_ItemInfo* pVerLine = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_VERLINE);
	NULL_CHECK_RETURN(pVerLine, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_VerLine", pVerLine), E_FAIL);

	CUI_ItemInfo* pHorLine = CUI_ItemInfo::Create(m_pGraphicDev, SHOPITEMTYPE::SHOP_HORLINE);
	NULL_CHECK_RETURN(pHorLine, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Shop_HorLine", pHorLine), E_FAIL);
	
	CNPCCow* pNPCCow = CNPCCow::Create(m_pGraphicDev, { 10, 1, 3 }, NPCTYPE::TUT_COW);
	NULL_CHECK_RETURN(pNPCCow, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Cow", pNPCCow), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

