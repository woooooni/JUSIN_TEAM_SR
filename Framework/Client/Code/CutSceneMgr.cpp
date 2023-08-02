#include "CutSceneMgr.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "UI_Dialog.h"
#include "GameMgr.h"
#include "SunGollem.h"
#include  "Npc_NueHero.h"
#include "SilkWorm.h"

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
	, m_fAccTimeNueHero_Intro(0.f)
	, m_fFinishTimeNueHero_Die(0.f)
	, m_fAccTimeNueHero_Die(0.f)
	, m_fFinishTimeNueHero_Intro(0.f)
	, m_bCutScenePlaying(false)
	,m_iEventNum(0)
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
	case CUTSCENE_TYPE::BOSS_SUNGOLEM_DIE:
		Update_Boss_SunGolem_Die(fTimeDelta);
		break;

	case CUTSCENE_TYPE::BOSS_NUEHERO_INTRO:
		Update_Boss_NueHero_Intro(fTimeDelta);
		break;

	case CUTSCENE_TYPE::BOSS_NUEHERO_DIE:
		Update_Boss_NueHero_Die(fTimeDelta);
		break;
	case CUTSCENE_TYPE::MONKEY2_HIT_ONE:
		Update_CutSceneMonkeyForest2(fTimeDelta);
		break;
	case CUTSCENE_TYPE::MONKEY2_HIT_TWO:
		Update_CutSceneMonkeyForest_OpenGate(fTimeDelta);
		break;
	case CUTSCENE_TYPE::MOON_FOREST_DOOR:
		Update_CutSceneMoonForestDoor(fTimeDelta);
		break;

	}
}

void CCutSceneMgr::Update_TutorialIntro()
{
	
		
}

void CCutSceneMgr::Update_MonkeyVillageIntro(const _float& fTimeDelta)
{
	m_fAccTimeMonkeyVillage += fTimeDelta;
	_vec3		vTargPos, vCamPos, vTmp, vSrc, vLook;
	m_pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);

	if (!m_bCutsceneSwitch[0])
	{
		
		vTargPos = { 68.2f,33.1f, 46.9f };
		vSrc = (vTargPos - vCamPos);
		if (D3DXVec3Length(&vSrc) < 0.2f)
			m_bCutsceneSwitch[0] = true;
		else
		{
			
			vTmp = (vTargPos - _vec3(129.5, 37.9, 132.6));
			vCamPos += *D3DXVec3Normalize(&vSrc, &vSrc) * D3DXVec3Length(&vTmp) * fTimeDelta * 0.2f;
			vLook = -(m_pCamera->Get_Offset()) + vCamPos;
		}
	}
	else if (!m_bCutsceneSwitch[1])
	{
		vTargPos = { 212.9f, 32.1f, 95.4f };
		vSrc = (vTargPos - vCamPos);
		if (D3DXVec3Length(&vSrc) < 0.2f)
			m_bCutsceneSwitch[1] = true;
		else
		{

			vTmp = (vTargPos - _vec3(68.2f, 33.1f, 46.9f));
			vCamPos += *D3DXVec3Normalize(&vSrc, &vSrc) * D3DXVec3Length(&vTmp) * fTimeDelta * 0.2f;
			vLook = -(m_pCamera->Get_Offset()) + vCamPos;
		}
	}
	else if (!m_bCutsceneSwitch[2])
	{

		vTargPos = { 141.2f, 32.1f, 49.f };
		vSrc = (vTargPos - vCamPos);
		if (D3DXVec3Length(&vSrc) < 0.2f)
			m_bCutsceneSwitch[2] = true;
		else
		{

			vTmp = (vTargPos - _vec3(212.9f, 32.1f, 95.4f));
			vCamPos += *D3DXVec3Normalize(&vSrc, &vSrc) * D3DXVec3Length(&vTmp) * fTimeDelta * 0.2f;
			vLook = -(m_pCamera->Get_Offset()) + vCamPos;
		}

	}
	else
	{
		/*_vec3 vPlayerPos;
		CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
		vTargPos = vPlayerPos + m_pCamera->Get_Offset();
		if (D3DXVec3Length(&(vCamPos - vTargPos)) < 0.1f)
			Finish_CutSceneMonkeyVillage();
		else
		{
			vTmp = (vTargPos - vCamPos);
			D3DXVec3Normalize(&vTmp, &vTmp);
			vCamPos += vTmp * fTimeDelta * 10.f;
			vLook = -(m_pCamera->Get_Offset()) + vCamPos;

		}*/
		Finish_CutSceneMonkeyVillage();
	}

	m_pCamera->Get_TransformCom()->Set_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Set_Info(INFO_LOOK, &vLook);

}

void CCutSceneMgr::Update_Boss_SunGolem_Intro(const _float& fTimeDelta)
{
	m_fAccTimeSunGolem += fTimeDelta;

	_vec3	vTarget ,vCamPos, vLook, vDir;
	m_pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);

	CSunGollem* sungollem = dynamic_cast<CSunGollem*>(Get_Layer(LAYER_TYPE::MONSTER)->Find_GameObject(L"SunGollem"));

	if (!sungollem || !sungollem->Is_Active())
		return;

	if (!m_bCutsceneSwitch[0])
	{
		sungollem->Get_TransformCom()->Get_Info(INFO_POS, &vTarget);
		vTarget += m_pCamera->Get_Offset();

		vDir = vTarget - vCamPos;
		

		if (D3DXVec3Length(&vDir) < 0.2f)
		{
			m_bCutsceneSwitch[0] = true;
			sungollem->Set_Stop(false);
			//썬골렘 애니메이션 활성화
			m_pCamera->Set_Offset(m_pCamera->Get_Offset() * 0.5f);
		}
		else
		{
			vCamPos += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * 15.f;
			vLook = vCamPos - m_pCamera->Get_Offset();
		}
	}
	else if (!m_bCutsceneSwitch[1] && sungollem->Get_Time() >= 5.f)
	{
		m_fFinishTimeSunGolem = m_fAccTimeSunGolem;
		m_pCamera->CamShake(1.5f, 1.f);
		m_bCutsceneSwitch[1] = true;
	}
	else if (m_bCutsceneSwitch[1] && m_fAccTimeSunGolem - m_fFinishTimeSunGolem >= 1.5f)
	{
		Finish_CutSceneSunGolem();
	}

	m_pCamera->Get_TransformCom()->Set_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Set_Info(INFO_LOOK, &vLook);


}

void CCutSceneMgr::Update_Boss_SunGolem_Die(const _float& fTimeDelta)
{
	m_fAccTimeSunGolem_Die += fTimeDelta;

	_vec3	vTarget, vCamPos, vLook, vDir;
	m_pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);

	CSunGollem* sungollem = dynamic_cast<CSunGollem*>(Get_Layer(LAYER_TYPE::MONSTER)->Find_GameObject(L"SunGollem"));

	if (!m_bCutsceneSwitch[0])
	{
		/*sungollem->Get_TransformCom()->Get_Info(INFO_POS, &vTarget);
		vTarget += m_pCamera->Get_Offset();

		vDir = vTarget - vCamPos;

		if (D3DXVec3Length(&vDir) < 0.1f)*/
		{
			m_bCutsceneSwitch[0] = true;
			m_pCamera->CamShake(3.f);
			m_pCamera->Set_TargetObj(sungollem);
			
			m_fFinishTimeSunGolem_Die = m_fAccTimeSunGolem_Die;
		}
		/*else
		{
			vCamPos += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * 20.f;
			vLook = vCamPos - m_pCamera->Get_Offset();
		}*/
	}
	else if (!m_bCutsceneSwitch[1] && sungollem->Get_Time() >= 5.f)
	{
		vTarget = { 8.9, 4.7, 20.5 };
		vTarget += m_pCamera->Get_Offset();

		vDir = vTarget - vCamPos;

		if (D3DXVec3Length(&vDir) < 0.3f)
		{
			m_bCutsceneSwitch[1] = true;
			Check_Event_Start(2);
			m_fFinishTimeSunGolem_Die = m_fAccTimeSunGolem_Die;


		}
		else
		{
			vCamPos += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * 10.f;
			vLook = vCamPos - m_pCamera->Get_Offset();
			m_pCamera->Set_TargetObj(nullptr);
		}

	}
	else if (m_bCutsceneSwitch[1] && (m_fAccTimeSunGolem_Die - m_fFinishTimeSunGolem_Die) >= 7.f)
	{
		Check_Event_Start(1);

		Finish_CutSceneSunGolem_Die();
	}

	m_pCamera->Get_TransformCom()->Set_Info(INFO_POS, &vCamPos);
	m_pCamera->Get_TransformCom()->Set_Info(INFO_LOOK, &vLook);



}

void CCutSceneMgr::Update_Boss_NueHero_Intro(const _float& fTimeDelta)
{
	m_fAccTimeNueHero_Intro += fTimeDelta;

	if (!m_bCutsceneSwitch[0] && CUIMgr::GetInstance()->Get_Fade()->Get_Finish())
	{
		dynamic_cast<CNpc_NueHero*>(Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Nue_NPC"))->Set_Summoned();
		CSilkWorm* pSilkWorm = CSilkWorm::Create(Engine::Get_Device());
		_vec3 vPos = _vec3(53.f, 0.5f, 26.f);
		pSilkWorm->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

		Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"SilkWorm", pSilkWorm);

		CUIMgr::GetInstance()->Get_Fade()->Set_Fade(false, 3.f);
		m_bCutsceneSwitch[0] = true;
	}
	else if (CUIMgr::GetInstance()->Get_Fade()->Get_Finish())
	{
		Finish_CutSceneNueHero_Intro();
	}

}

void CCutSceneMgr::Update_Boss_NueHero_Die(const _float& fTimeDelta)
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
		dia->Get_StringVec().clear();
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

void CCutSceneMgr::Update_CutSceneMoonForestDoor(const _float& fTimeDelta)
{
	m_fAccTimeMoonForestDoor += fTimeDelta;

	if (!m_bCutsceneSwitch[0])
	{
		m_pCamera->Set_TargetObj(Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NextPotal"));
		m_bCutsceneSwitch[0] = TRUE;
	}
	else if (!m_bCutsceneSwitch[1] && m_fAccTimeMoonForestDoor >= 3.f)
	{
		Check_Event_Start(m_iEventNum);
		m_bCutsceneSwitch[1] = TRUE;
	}
	else if (m_bCutsceneSwitch[1] && m_fAccTimeMoonForestDoor >= 7.f)
	{
		Finish_CutSceneMonkeyForest_OpenGate();
	}

}

void CCutSceneMgr::Update_CutSceneMonkeyForest_OpenGate(const _float& fTimeDelta)
{
	m_fAccTimeMonkeyForest2 += fTimeDelta;

	if (!m_bCutsceneSwitch[0])
	{
		m_pCamera->Set_TargetObj(Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Find_GameObject(L"Block_CutScene"));
		m_bCutsceneSwitch[0] = TRUE;
	}
	else if (!m_bCutsceneSwitch[1] && m_fAccTimeMonkeyForest2 >= 3.f)
	{
		Check_Event_Start(m_iEventNum);
		m_bCutsceneSwitch[1] = TRUE;
	}
	else if (m_bCutsceneSwitch[1] && m_fAccTimeMonkeyForest2 >= 5.f)
	{
		Finish_CutSceneMonkeyForest_OpenGate();
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
	m_bCutsceneSwitch.resize(3);
	m_pCamera->Set_TargetObj(nullptr);
	m_pCamera->Get_TransformCom()->Set_Pos(&_vec3(129.5, 37.9, 132.6));

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);

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
	m_pCamera->Set_TargetObj(nullptr);
	m_pCamera->Set_Offset(m_pCamera->Get_Offset() * 2.f);
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);

}

void CCutSceneMgr::Ready_CutSceneSunGolem_Die()
{
	m_fAccTimeSunGolem_Die = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

	m_fFinishTimeSunGolem = 0.f;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	m_pCamera->Set_TargetObj(nullptr);
	m_pCamera->Set_Offset(m_pCamera->Get_Offset() * 2.f);
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);

}

void CCutSceneMgr::Ready_CutSceneNueHero_Intro()
{
	m_fAccTimeNueHero_Intro = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

	m_fFinishTimeSunGolem = 0.f;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	m_pCamera->Set_TargetObj(nullptr);
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);
	m_pCamera->CamShake(3.f);
	CUIMgr::GetInstance()->Get_Fade()->Set_Fade(true, 3.f);

}

void CCutSceneMgr::Ready_CutSceneNueHero_Die()
{
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

	m_fFinishTimeMonkeyForest2 = 0.f;
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);
	
	Find_Timer(L"WorldTimer_FPS60")->Set_TimeScale(0.f);

}

void CCutSceneMgr::Ready_CutSceneMonkeyForest_OpenGate()
{
	m_fAccTimeMonkeyForest2 = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	m_pCamera->Set_TargetObj(nullptr);
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);

}

void CCutSceneMgr::Ready_CutSceneMoonForestDoor()
{
	m_fAccTimeMoonForestDoor = 0.f;
	m_pCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (nullptr == m_pCamera)
		return;

	m_pCamera->Set_CameraState(CAMERA_STATE::CUT_SCENE);
	m_pCamera->Set_TargetObj(nullptr);
	m_bCutsceneSwitch.clear();
	m_bCutsceneSwitch.resize(2);

	CGameMgr::GetInstance()->Get_Player()->Set_Stop(true);

}

void CCutSceneMgr::Finish_CutSceneTutorial()
{
	m_bCutScenePlaying = false;
}

void CCutSceneMgr::Finish_CutSceneMonkeyVillage()
{
	m_bCutScenePlaying = false;
	m_bCutsceneSwitch.clear();
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);


}

void CCutSceneMgr::Finish_CutSceneSunGolem()
{
	m_bCutScenePlaying = false;
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);

}

void CCutSceneMgr::Finish_CutSceneSunGolem_Die()
{
	m_bCutScenePlaying = false;
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);
}

void CCutSceneMgr::Finish_CutSceneNueHero_Intro()
{
	m_bCutScenePlaying = false;
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);

}

void CCutSceneMgr::Finish_CutSceneNueHero_Die()
{
}


void CCutSceneMgr::Finish_CutSceneMonkeyForest2()
{
	m_bCutScenePlaying = false;

	m_bCutsceneSwitch.clear();

	if (m_iEventNum)
	{
		Check_Event_Start(m_iEventNum);
	}

	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);

	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);


}

void CCutSceneMgr::Finish_CutSceneMoonForestDoor()
{
	m_bCutScenePlaying = false;
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());
	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);

}

void CCutSceneMgr::Finish_CutSceneMonkeyForest_OpenGate()
{
	m_bCutScenePlaying = false;
	m_pCamera->Set_CameraState(CAMERA_STATE::GAME);
	m_pCamera->Set_TargetObj(CGameMgr::GetInstance()->Get_Player());

	CGameMgr::GetInstance()->Get_Player()->Set_Stop(false);

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
		Ready_CutSceneNueHero_Intro();
		break;

	case CCutSceneMgr::CUTSCENE_TYPE::BOSS_NUEHERO_DIE:
		Ready_CutSceneNueHero_Die();
		break;

	case CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_ONE:
		Ready_CutSceneMonkeyForest2();
		break;

	case CCutSceneMgr::CUTSCENE_TYPE::BOSS_SUNGOLEM_DIE:
		Ready_CutSceneSunGolem_Die();
		break;

	case CCutSceneMgr::CUTSCENE_TYPE::MONKEY2_HIT_TWO:
		Ready_CutSceneMonkeyForest_OpenGate();
		break;

	case CUTSCENE_TYPE::MOON_FOREST_DOOR:
		Ready_CutSceneMoonForestDoor();

	case CCutSceneMgr::CUTSCENE_TYPE::TYPE_END:
		break;
	default:
		break;
	}
}

void CCutSceneMgr::Free()
{
}
