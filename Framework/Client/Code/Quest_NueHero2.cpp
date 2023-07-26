#include "Quest_NueHero2.h"

CQuest_NueHero2::CQuest_NueHero2()
	:CQuest_Conversation(NPC_CODE::NPC_NUE_HERO, nullptr)
{
	// 순수 누에용사
	m_strQuestTitle = L"누에용사와 대화하기.";
	m_strQuestDesc = L"타락한 누에용사를 혼내줬다. 순수해진 누에용사와 대화해보자.";

	// TODO :: 누에용사 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"");
	
}

void CQuest_NueHero2::Update_Quest(_float& fTimeDelta)
{
	
}

void CQuest_NueHero2::Accept_Quest()
{
	Clear_Quest();
}

void CQuest_NueHero2::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	// TODO :: 컷씬 후 게임 종료
}
