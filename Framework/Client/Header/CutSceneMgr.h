#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CCamera;
END
class CCutSceneMgr : public CBase
{
	DECLARE_SINGLETON(CCutSceneMgr)

public:
	enum class CUTSCENE_TYPE { TUTORIAL_INTRO, MONKEY_VILLAGE_INTRO, BOSS_SUNGOLEM_INTRO, BOSS_NUEHERO_INTRO, TYPE_END };

private:
	explicit CCutSceneMgr();
	virtual ~CCutSceneMgr();

public:
	HRESULT Ready_CutSceneMgr();
	void Update_CutSceneMgr(const _float& fTimeDelta);

public:
	void Start_CutScene(CUTSCENE_TYPE _eType) { Ready_CutScene(_eType);  m_eCurrCutScene = _eType; }
	_bool Is_Playing() { return m_bCutScenePlaying; }

private:
	void Ready_CutScene(CUTSCENE_TYPE _eType);

private:
	void Ready_CutSceneTutorial();
	void Ready_CutSceneMonkeyVillage();
	void Ready_CutSceneSunGolem();
	void Ready_CutSceneNueHero();

private:
	void Update_TutorialIntro();
	void Update_MonkeyVillageIntro();
	void Update_Boss_SunGolem_Intro();
	void Update_Boss_NueHero_Intro();

private:
	void Finish_CutSceneTutorial();
	void Finish_CutSceneMonkeyVillage();
	void Finish_CutSceneSunGolem();
	void Finish_CutSceneNueHero();



private:
	CUTSCENE_TYPE m_eCurrCutScene;

	_float m_fAccTimeTutorial;
	_float m_fFinishTimeTutorial;

	_float m_fAccTimeMonkeyVillage;
	_float m_fFinishTimeMonkeyVillage;

	_float m_fAccTimeSunGolem;
	_float m_fFinishTimeSunGolem;

	_float m_fAccTimeNueHero;
	_float m_fFinishTimeNueHero;

	_bool m_bCutScenePlaying;

	CCamera* m_pCamera;

public:
	virtual void Free() override;

};

