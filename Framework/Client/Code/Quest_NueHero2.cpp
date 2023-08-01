#include "UIMgr.h"
#include "Quest_NueHero2.h"

CQuest_NueHero2::CQuest_NueHero2()
	:CQuest_Conversation(NPC_CODE::NPC_NUE_HERO, nullptr)
{
	// 순수 누에용사
	m_strQuestTitle = L"누에용사와 대화하기.";
	//m_strQuestSub = L""; // 생략
	m_strQuestDesc = L"타락한 누에용사를 혼내줬다. 순수해진 누에용사와 대화해보자.";

	// TODO :: 누에용사 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"어떻게 된거지?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"뽕잎을 먹고 기절한 뒤로 기억이 나질 않아.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"공장에서 방류한 오염수 때문에\n돌연변이 식물이 자라난단 이야길 들었어.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"아무래도 우리 마을 식물들에 생긴 것 같은데. 앞으로 어쩌지...");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"네가 날 구해준 것 같구나! 이 은혜를 어떻게 갚지?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"앞으로 너희 마을에 쓰레기를 버리는 일은 없을거야.");
	// 오글거려서 더는 못쓰겠어요 ^^

	//m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");

	//m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"");
	
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
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	// TODO :: 컷씬 후 게임 종료
}
