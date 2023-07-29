#include "CutSceneMgr.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "UI_Dialog.h"
#include "GameMgr.h"

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
	if (!m_bCutScenePlaying)
		return;

	switch (m_eCurrCutScene)
	{
	case CUTSCENE_TYPE::TUTORIAL_INTRO:
		Update_TutorialIntro();
		break;

	case CUTSCENE_TYPE::MONKEY_VILLAGE_INTRO:
		Update_MonkeyVillageIntro(fTimeDelta);
		break;

	case CUTSCENE_TYPE::BOSS_SUNGOLEM_INTRO:
		Update_Boss_SunGolem_Intro(fTimeDelta);
		break;

	case CUTSCENE_TYPE::BOSS_NUEHERO_INTRO:
		Update_Boss_NueHero_Intro();
		break;
	case CUTSCENE_TYPE::MONKEY2_HIT_ONE:
		Update_CutSceneMonkeyForest2(fTimeDelta);
		break;
	}
}

void CCutSceneMgr::Update_TutorialIntro()
{
	
		
}

void CCutSceneMgr::Update_MonkeyVillageIntro(const _float& fTimeDelta)
{
	m_fAccTimeMonkeyVillage += fTimeDelta;

	if (m_fAccTimeMonkeyVillage > 0.f && !m_bCutsceneSwitch[0])
	{
		CGameObject* followTarget = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Chief");
		m_pCamera->Set_TargetObj(followTarget);		
		m_bCutsceneSwitch[0] = true;
	}
	else if (m_fAccTimeMonkeyVillage > 3.f && !m_bCutsceneSwitch[1])
	{
		m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
		Finish_CutSceneMonkeyVillage();
	}
}

void CCutSceneMgr::Update_Boss_SunGolem_Intro(const _float& fTimeDelta)
{
}

void CCutSceneMgr::Update_Boss_NueHero_Intro()
{
}

void CCutSceneMgr::Update_CutSceneMonkeyForest2(const _float&	fTimeDelta)
{
	m_fAccTimeMonkeyForest2 += fTimeDelta;
	CUI_Dialog* dia = CUIMgr::GetInstance()->Get_Dialog();

	if (m_fAccTimeMonkeyForest2 >= 0.f && !m_bCutsceneSwitch[0])
	{
		m_pCamera->CamShake(1.f, 0.5f);
		m_bCutsceneSwitch[0] = true;
	}
	else if (m_fAccTimeMonkeyForest2 >= 1.f && !m_bCutsceneSwitch[1])
	{
		dia->Set_Name(L"시스템 메세지");
		dia->Get_StringVec().push_back(L"어디선가 쿵 하는 소리가 났다.");
		dia->Set_Quest(nullptr);
		dia->Set_Active(true);

		m_bCutsceneSwitch[1] = true;
	}
	else if (m_fAccTimeMonkeyForest2 > 1.f && m_bCutsceneSwitch[1] && !dia->Is_Active())
	{
		Finish_CutSceneMonkeyForest2();
	}
	


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
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
}

void CCutSceneMgr::Ready_CutSceneSunGolem()
{
	m_fAccTimeSunGolem = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

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

void CCutSceneMgr::Ready_CutSceneMonkeyForest2()
{
	m_fAccTimeMonkeyForest2 = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);
	
	Find_Timer(L"WorldTimer_FPS60")->Set_TimeScale(0.f);

}

void CCutSceneMgr::Finish_CutSceneTutorial()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneMonkeyVillage()
{
	m_bCutScenePlaying = false;
	m_bCutsceneSwitch.clear();

}

void CCutSceneMgr::Finish_CutSceneSunGolem()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneNueHero()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneMonkeyForest2()
{
	m_bCutScenePlaying = false;

	m_bCutsceneSwitch.clear();

	Check_Event_Start(17);

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

	case CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE:
		Ready_CutSceneMonkeyForest2();
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
