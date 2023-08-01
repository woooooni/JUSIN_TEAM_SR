#include "Export_Function.h"
#include "Quest_MonkeyCheif1.h"
#include "Quest_MonkeyCheif2.h"
#include "QuestMgr.h"
#include "UIMgr.h"

CQuest_MonkeyCheif1::CQuest_MonkeyCheif1()
	:CQuest_Conversation(NPC_CODE::NPC_CHIEF, nullptr)
{
	// 원숭이 촌장에게 말을 걸면 퀘스트 완료.
	m_strQuestTitle = L"원숭이 마을 촌장";
	m_strQuestDesc = L"원숭이들이 의심스럽다. 촌장을 만나보자.";

	// TODO : 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"원숭이 마을에 웬.. 오리너구리?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"우리 마을 주민들이 그쪽 마을에 쓰레기를 버린다고?");

//	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"");
//	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");
	
}

void CQuest_MonkeyCheif1::Update_Quest(_float& fTimeDelta)
{
	Clear_Quest();
}

void CQuest_MonkeyCheif1::Accept_Quest()
{
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;

	CUIMgr::GetInstance()->Get_QuestList()->Set_Shown(false);
}

void CQuest_MonkeyCheif1::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_MonkeyCheif2* pNextQuest = new CQuest_MonkeyCheif2();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}
