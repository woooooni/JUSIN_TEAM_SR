#include "Quest_Sheep.h"
#include "QuestMgr.h"
#include "Quest_Cow1.h"
#include "UIMgr.h"
CQuest_Sheep::CQuest_Sheep()
	:CQuest_Conversation(NPC_CODE::NPC_SHEEP, nullptr)
{
	m_strQuestTitle = L"불어난 쓰레기";
	m_strQuestSub = L"철물점 소 아저씨 찾아가기";
	m_strQuestDesc = L"궁금한 건 못 참는 아기오구!\n소 아저씨가 본 것이 무엇인지 물어보자.";

	// TODO : 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"오구 안녕? 좋은 아침이야~");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"어디 놀러나가는 거니?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"듣자 하니 요즘 마을 외곽이\n정체불명의 쓰레기들로 어수선하다고 하던데..");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"옆집 철물점 소 아저씨는\n마을 외곽에서 이상한 걸 봤다고 하더라.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"아무튼, 너무 늦은 시간까지 돌아다니지 말고!\n아무나 따라가면 큰일 난다~");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"소 아저씨가 뭘 봤는지 궁금하지 않니?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"우리 숲에 쓰레기를 버리는 녀석들이 있다고?");
}

void CQuest_Sheep::Update_Quest(_float& fTimeDelta)
{
}

void CQuest_Sheep::Accept_Quest()
{
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	Clear_Quest();

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Reserve_Active(true);

	CUIMgr::GetInstance()->Get_QuestList()->Set_Title(m_strQuestSub);
	CUIMgr::GetInstance()->Get_QuestList()->Set_Active(true);
}

void CQuest_Sheep::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_Cow1* pQuest = new CQuest_Cow1();
	CQuestMgr::GetInstance()->Add_Quest(pQuest);
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}
