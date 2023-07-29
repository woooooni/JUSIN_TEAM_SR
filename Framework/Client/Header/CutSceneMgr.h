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
	enum class CUTSCENE_TYPE { TUTORIAL_INTRO, MONKEY_VILLAGE_INTRO, BOSS_SUNGOLEM_INTRO, BOSS_NUEHERO_INTRO, MONKEY2_HIT_ONE, MONKEY2_HIT_TWO ,TYPE_END };

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
	void	Ready_CutSceneMonkeyForest2();

private:
	void Update_TutorialIntro();
	void Update_MonkeyVillageIntro(const _float& fTimeDelta);
	void Update_Boss_SunGolem_Intro(const _float& fTimeDelta);
	void Update_Boss_NueHero_Intro();
	void	Update_CutSceneMonkeyForest2(const _float& fTimeDelta);

private:
	void Finish_CutSceneTutorial();
	void Finish_CutSceneMonkeyVillage();
	void Finish_CutSceneSunGolem();
	void Finish_CutSceneNueHero();
	void	Finish_CutSceneMonkeyForest2();



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

	_float m_fAccTimeMonkeyForest2;
	_float m_fFinishTimeMonkeyForest2;

	_bool m_bCutScenePlaying;

	CCamera* m_pCamera;

	vector<_bool>m_bCutsceneSwitch;

public:
	virtual void Free() override;

};

