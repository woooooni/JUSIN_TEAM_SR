#include "Export_Function.h"
#include "Quest_MonkeyCheif3.h"
#include "QuestMgr.h"
#include "Quest_NueHero1.h"
#include "GameMgr.h"

CQuest_MonkeyCheif3::CQuest_MonkeyCheif3()
	:CQuest_Conversation(NPC_CODE::NPC_CHIEF, nullptr)
{
	// 골렘이 사라져도 달라진 것이 없다. 진짜 범인을 찾으러 달숲으로 간다.
	m_strQuestTitle = L"진짜 범인은 누구?";
	m_strQuestDesc = L"아무래도 골렘이 범인은 아닌것 같다. 달숲을 조사해보자.";

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"쓰레기 냄새가 점점 더 심해지는 군");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"의심이 가는 곳이 더 있는데 오리너구리 네가 가볼 텐가?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"사양하지 말게. 교통편도 걱정하지 말도록.");
	// m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"바람과 풍선만 있다면 어디든 갈 수 있다네.");

	// m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"바람과 풍선만 있다면 어디든 갈 수 있다네.");

	m_iRewardCoin = 500;
}

void CQuest_MonkeyCheif3::Update_Quest(_float& fTimeDelta)
{;
	CScene* pScene = Engine::GetCurrScene();
	if (pScene != nullptr)
	{
		if (pScene->Get_SceneType() == SCENE_TYPE::MOON_FOREST1)
			Clear_Quest();
	}
}

void CQuest_MonkeyCheif3::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if(nullptr != pPlayer)
		pPlayer->Change_State_Now(PLAYER_STATE::BALLOONFLY);

}

void CQuest_MonkeyCheif3::Clear_Quest()
{
	CQuest_NueHero1* pNextQuest = new CQuest_NueHero1();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return;

	pPlayer->Add_Money(m_iRewardCoin);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	
}
