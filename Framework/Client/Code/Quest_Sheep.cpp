#include "Quest_Sheep.h"
#include "QuestMgr.h"
#include "Quest_Cow1.h"
CQuest_Sheep::CQuest_Sheep()
	:CQuest_Conversation(NPC_CODE::NPC_SHEEP, nullptr)
{
	m_strQuestTitle = L"불어난 쓰레기";
	m_strQuestDesc = L"소 아저씨에게 쓰레기가 늘어난 이유에 대해 물어보자.";

	// TODO : 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"오구 안녕?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"좋은 아침이야~");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"");
}

void CQuest_Sheep::Update_Quest(_float& fTimeDelta)
{
	
}

void CQuest_Sheep::Accept_Quest()
{
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	Clear_Quest();
}

void CQuest_Sheep::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_Cow1* pQuest = new CQuest_Cow1();
	CQuestMgr::GetInstance()->Add_Quest(pQuest);
}
