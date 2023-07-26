#include "CutSceneMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCutSceneMgr)

CCutSceneMgr::CCutSceneMgr()
	: m_eCurrCutScene(CUTSCENE_TYPE::TYPE_END)
	, m_pCamera(nullptr)
	, m_fAccTimeTutorial(0.f)
	, m_fFinishTimeTutorial(0.f)
	, m_fAccTimeMonkeyVillage(0.f)
	, m_fFinishTimeMonkeyVillage(0.f)
	, m_fAccTimeSunGolem(0.f)
	, m_fFinishTimeSunGolem(0.f)
	, m_fAccTimeNueHero(0.f)
	, m_fFinishTimeNueHero(0.f)
	, m_bCutScenePlaying(false)
{

}

CCutSceneMgr::~CCutSceneMgr()
{
	Free();
}

HRESULT CCutSceneMgr::Ready_CutSceneMgr()
{
	return S_OK;
}

void CCutSceneMgr::Update_CutSceneMgr(const _float& fTimeDelta)
{
	switch (m_eCurrCutScene)
	{
	case CUTSCENE_TYPE::TUTORIAL_INTRO:
		Update_TutorialIntro();
		break;

	case CUTSCENE_TYPE::MONKEY_VILLAGE_INTRO:
		Update_MonkeyVillageIntro();
		break;

	case CUTSCENE_TYPE::BOSS_SUNGOLEM_INTRO:
		Update_Boss_SunGolem_Intro();
		break;

	case CUTSCENE_TYPE::BOSS_NUEHERO_INTRO:
		Update_Boss_NueHero_Intro();
		break;
	}
}

void CCutSceneMgr::Update_TutorialIntro()
{
	
		
}

void CCutSceneMgr::Update_MonkeyVillageIntro()
{

}

void CCutSceneMgr::Update_Boss_SunGolem_Intro()
{
}

void CCutSceneMgr::Update_Boss_NueHero_Intro()
{
}

void CCutSceneMgr::Ready_CutSceneTutorial()
{
	m_fAccTimeTutorial = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
}

void CCutSceneMgr::Ready_CutSceneMonkeyVillage()
{
	m_fAccTimeMonkeyVillage = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
}

void CCutSceneMgr::Ready_CutSceneSunGolem()
{
	m_fAccTimeSunGolem = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
}

void CCutSceneMgr::Ready_CutSceneNueHero()
{
	m_fAccTimeNueHero = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
}

void CCutSceneMgr::Finish_CutSceneTutorial()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneMonkeyVillage()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneSunGolem()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneNueHero()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Ready_CutScene(CUTSCENE_TYPE _eType)
{
	m_bCutScenePlaying = true;

	switch (_eType)
	{
	case CCutSceneMgr::CUTSCENE_TYPE::TUTORIAL_INTRO:
		Ready_CutSceneTutorial();
		break;
	case CCutSceneMgr::CUTSCENE_TYPE::MONKEY_VILLAGE_INTRO:
		Ready_CutSceneMonkeyVillage();
		break;
	case CCutSceneMgr::CUTSCENE_TYPE::BOSS_SUNGOLEM_INTRO:
		Ready_CutSceneSunGolem();
		break;
	case CCutSceneMgr::CUTSCENE_TYPE::BOSS_NUEHERO_INTRO:
		Ready_CutSceneNueHero();
		break;
	case CCutSceneMgr::CUTSCENE_TYPE::TYPE_END:
		break;
	default:
		break;
	}
}

void CCutSceneMgr::Free()
{
}
