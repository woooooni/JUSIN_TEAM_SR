#include "Export_Function.h"
#include "Quest_MonkeyCheif3.h"
#include "QuestMgr.h"
#include "Quest_NueHero1.h"
#include "GameMgr.h"

CQuest_MonkeyCheif3::CQuest_MonkeyCheif3()
	:CQuest_Conversation(NPC_CODE::NPC_CHIEF, nullptr)
{
	// ���� ������� �޶��� ���� ����. ��¥ ������ ã���� �޽����� ����.
	m_strQuestTitle = L"��¥ ������ ����?";
	m_strQuestDesc = L"�ƹ����� ���� ������ �ƴѰ� ����. �޽��� �����غ���.";

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ ������ ���� �� �������� ��");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�ǽ��� ���� ���� �� �ִµ� �����ʱ��� �װ� ���� �ٰ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������� ����. ������ �������� ������.");
	// m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�ٶ��� ǳ���� �ִٸ� ���� �� �� �ִٳ�.");

	// m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"�ٶ��� ǳ���� �ִٸ� ���� �� �� �ִٳ�.");

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
