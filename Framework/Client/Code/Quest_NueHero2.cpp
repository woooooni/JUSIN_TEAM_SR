#include "Quest_NueHero2.h"

CQuest_NueHero2::CQuest_NueHero2()
	:CQuest_Conversation(NPC_CODE::NPC_NUE_HERO, nullptr)
{
	m_strQuestTitle = L"누에용사와 대화하기.";
	m_strQuestDesc = L"타락한 누에용사를 혼내줬다. 순수해진 누에용사와 대화해보자.";

	// TODO :: 누에용사 대사 쓰기
	
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
