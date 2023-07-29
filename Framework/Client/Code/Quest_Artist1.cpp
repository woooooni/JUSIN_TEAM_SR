#include "Quest_Artist1.h"
#include "GameMgr.h"
CQuest_Artist1::CQuest_Artist1(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	:CQuest_Conversation(NPC_CODE::NPC_ARTIST, nullptr)
{
	m_strQuestTitle = L"그림 그리기";
	m_strQuestDesc = L"삼원색에 대해 배워보자";

	// TODO : 대사 쓰기
	//m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)];
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"와우! 그림에 소질이 있는걸?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"앞으로도 이 표시를 발견하면\n그냥 지나치지 말고 그림을 그려보도록 해!");
}

void CQuest_Artist1::Update_Quest(_float& fTimeDelta)
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer != nullptr)
	{
		// TODO :: GetState & Clear Quest
	}
}

void CQuest_Artist1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
}

void CQuest_Artist1::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
}
