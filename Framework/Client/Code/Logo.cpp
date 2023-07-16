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
	Check_Event_Start(10);

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



	pPlayer->Set_Hat(pItemMaskHat);

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

	//CNpc_Cow* pNPCCow = CNpc_Cow::Create(m_pGraphicDev, { 10, 1, 3 }, NPCTYPE::TUT_COW);
	//NULL_CHECK_RETURN(pNPCCow, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Cow", pNPCCow), E_FAIL);

	//CNpcSheep* pNPCSheep = CNpcSheep::Create(m_pGraphicDev, { 18, 1, 7 }, NPCTYPE::TUT_SHEEP);
	//NULL_CHECK_RETURN(pNPCSheep, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Sheep", pNPCSheep), E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CPushStone* pPush = CPushStone::Create(_vec3(1, 1, 3), m_pGraphicDev);
	NULL_CHECK_RETURN(pPush, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pPush), E_FAIL);

	CHoleObj* pHole = CHoleObj::Create(m_pGraphicDev, 0, {1, 1, 4});
	NULL_CHECK_RETURN(pHole, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hole", pHole), E_FAIL);


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

	CQuizStatue* pQuiz = CQuizStatue::Create(m_pGraphicDev, 7, CQuizStatue::MONKEY_STATUE::EAR, { 16, 0, 20 });
	NULL_CHECK_RETURN(pQuiz, E_FAIL);
	Add_Subscribe(10, pQuiz);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuizStatue", pQuiz), E_FAIL);

	pQuiz = CQuizStatue::Create(m_pGraphicDev, 8, CQuizStatue::MONKEY_STATUE::MOUTH, { 17, 0, 20 });
	NULL_CHECK_RETURN(pQuiz, E_FAIL);
	Add_Subscribe(10, pQuiz);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuizStatue", pQuiz), E_FAIL);

	pQuiz = CQuizStatue::Create(m_pGraphicDev, 9, CQuizStatue::MONKEY_STATUE::EYE, { 18, 0, 20 });
	NULL_CHECK_RETURN(pQuiz, E_FAIL);
	Add_Subscribe(10, pQuiz);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuizStatue", pQuiz), E_FAIL);



	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 11, { 15, 1, 20 }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlockObj", pBlock), E_FAIL);

	CBreakStone* pBreakStone = CBreakStone::Create(m_pGraphicDev,JELLY_COLOR::YELLOW, 2, { 16, 1, 20 });
	NULL_CHECK_RETURN(pBreakStone, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlockObj", pBreakStone), E_FAIL);

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

	CDefaultItem* pEtc = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::TWIG);
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

	pLayer->Ready_Layer();

	return S_OK;
}

