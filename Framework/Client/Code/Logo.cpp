#include "..\Header\Logo.h"
#include "Export_Function.h"
#include "CUI.h"
#include "../Include/stdafx.h"
#include "SkyBox.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::LOADING)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYER_TYPE::ENVIRONMENT), E_FAIL);

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pGraphicDev->SetViewport(&vp);

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

HRESULT CLogo::Ready_Layer_Environment(LAYER_TYPE _eType)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer), E_FAIL);

	// Camera
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pCamera), E_FAIL);

	//pCamera->Set_CameraState(CAMERA_STATE::TOOL);

	// BlueBeatle
	CBlueBeatle* pBlueBeatle = CBlueBeatle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pBlueBeatle, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlueBeatle", pBlueBeatle), E_FAIL);
	
	/*CTrashBig* pTrashBig = CTrashBig::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTrashBig, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CTrashBig", pTrashBig), E_FAIL);*/

	CDesertRhino* pDesertRhino = CDesertRhino::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pDesertRhino, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DesertRhino", pDesertRhino), E_FAIL);
	
	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SunGollem", pSunGollem), E_FAIL);

	CPushStone* pPush = CPushStone::Create(_vec3(3, 1, 3), m_pGraphicDev);
	NULL_CHECK_RETURN(pPush, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pPush), E_FAIL);


	CNearReactObj* pNear = CNearReactObj::Create(m_pGraphicDev, {5, 1, 5});
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




	CGrabbableObj* pGrab = CGrabbableObj::Create(m_pGraphicDev, { 7, 1, 7 });
	NULL_CHECK_RETURN(pGrab, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GrabStone", pGrab), E_FAIL);

	CBalpanObj* pBal = CBalpanObj::Create(m_pGraphicDev, 4, { 13, 1, 15 });
	NULL_CHECK_RETURN(pBal, E_FAIL);
	pBal->Set_AutoReset();
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


	CHitObj* pHit = CHitObj::Create(m_pGraphicDev, 0, { 17, 1, 17 });
	NULL_CHECK_RETURN(pHit, E_FAIL);
	pHit->Set_Event(2);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugDongsang", pHit), E_FAIL);

	CBlockObj* pBlock = CBlockObj::Create(m_pGraphicDev, 4, { 15, 1, 20 }, true);
	NULL_CHECK_RETURN(pBlock, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlockObj", pBlock), E_FAIL);

	// SkyBox
	CSkyBox* pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pSkyBox), E_FAIL);

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

	CLightPuzzleBase* pLBase = CLightPuzzleBase::Create(m_pGraphicDev, 0, {25, 1, 25});
	NULL_CHECK_RETURN(pLBase, E_FAIL);
	pLBase->Reverse_Puzzle(true);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzleBase", pLBase), E_FAIL);

	CLightPuzzlePiece* pLPiece = CLightPuzzlePiece::Create(m_pGraphicDev, 0, { 28, 1, 28 });
	NULL_CHECK_RETURN(pLPiece, E_FAIL);
	pLBase->Reverse_Puzzle(true);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightPuzzleBase", pLPiece), E_FAIL);


	CCoin* pCoin = CCoin::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCoin, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Coin", pCoin), E_FAIL);

	CPlayer_Skill_Range* pPlayerSkillRange = CPlayer_Skill_Range::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayerSkillRange, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Skill_Range", pPlayerSkillRange), E_FAIL);
	pPlayer->Set_SkillRange(pPlayerSkillRange);

	CPlayer_Skill_Aim* pPlayerSkillAim = CPlayer_Skill_Aim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayerSkillAim, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Skill_Aim", pPlayerSkillAim), E_FAIL);
	pPlayer->Set_Aim(pPlayerSkillAim);


	// UI
	//CUI* pUI = CUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pUI, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pUI), E_FAIL);

	CNPCText* pTextBox = CNPCText::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTextBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_TextBox", pTextBox), E_FAIL);

	CTextBox* pText = CTextBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pText, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_TextBox", pText), E_FAIL);

	CIcon* pIconHeart = CIcon::Create(m_pGraphicDev, ICONTYPE::HEART);
	NULL_CHECK_RETURN(pIconHeart, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Heart", pIconHeart), E_FAIL);

	CIcon* pIconKeyboard = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBOARD);
	NULL_CHECK_RETURN(pIconKeyboard, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Keyboard", pIconKeyboard), E_FAIL);

	CIcon* pIconQuest = CIcon::Create(m_pGraphicDev, ICONTYPE::QUEST);
	NULL_CHECK_RETURN(pIconQuest, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_Heart", pIconQuest), E_FAIL);

	CIcon* pIconHPFrame = CIcon::Create(m_pGraphicDev, ICONTYPE::PLAYERHP_FRAME);
	NULL_CHECK_RETURN(pIconHPFrame, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_HPFrame", pIconHPFrame), E_FAIL);

	CIcon* pIconKeyButton = CIcon::Create(m_pGraphicDev, ICONTYPE::KEYBUTTON);
	NULL_CHECK_RETURN(pIconKeyButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_KeyButton", pIconKeyButton), E_FAIL);

	CQuickSlot* pQuickSlot = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pQuickSlot, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Icon_QuickSlot", pQuickSlot), E_FAIL);


	// NPC (Test)
	CTutorialNPC* pNPCSheep = CTutorialNPC::Create(m_pGraphicDev, { 2, 1, 1 }, NPCTYPE::TUT_SHEEP);
	NULL_CHECK_RETURN(pNPCSheep, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Sheep", pNPCSheep), E_FAIL);
	
	CTutorialNPC* pNPCCow = CTutorialNPC::Create(m_pGraphicDev, { 3, 1, 1 }, NPCTYPE::TUT_COW);
	NULL_CHECK_RETURN(pNPCCow, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Cow", pNPCCow), E_FAIL);

	CTutorialNPC* pNPCPig = CTutorialNPC::Create(m_pGraphicDev, { 4, 1, 1 }, NPCTYPE::TUT_PIG);
	NULL_CHECK_RETURN(pNPCPig, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Pig", pNPCPig), E_FAIL);

	CTutorialNPC* pNPCDoogee = CTutorialNPC::Create(m_pGraphicDev, { 5, 1, 1 }, NPCTYPE::TUT_DOOGEE);
	NULL_CHECK_RETURN(pNPCDoogee, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Tutorial_Doogee", pNPCDoogee), E_FAIL);

	pCamera->Set_TargetObj(pPlayer);

	m_mapLayer.insert({ _eType, pLayer });

	return S_OK;
}
