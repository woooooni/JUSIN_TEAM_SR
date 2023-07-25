#include "Export_Function.h"
#include "Quest_MonkeyCheif1.h"
#include "Quest_MonkeyCheif2.h"
#include "QuestMgr.h"

CQuest_MonkeyCheif1::CQuest_MonkeyCheif1()
	:CQuest_Conversation(NPC_CODE::NPC_CHIEF, nullptr)
{
	m_strQuestTitle = L"동굴 조사";
	m_strQuestDesc = L"마을 근처 동굴에서 계속 쓰레기가 나오는거 같다. 조사해보자.";

	// TODO : 대사 쓰기
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)];
}

void CQuest_MonkeyCheif1::Update_Quest(_float& fTimeDelta)
{
	
}

void CQuest_MonkeyCheif1::Accept_Quest()
{
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	Clear_Quest();
}

void CQuest_MonkeyCheif1::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_MonkeyCheif2* pNextQuest = new CQuest_MonkeyCheif2();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);
}
