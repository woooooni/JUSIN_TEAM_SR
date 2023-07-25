#include "Quest_Sheep.h"
#include "QuestMgr.h"
#include "Quest_Cow1.h"
CQuest_Sheep::CQuest_Sheep()
	:CQuest_Conversation(NPC_CODE::NPC_SHEEP, nullptr)
{
	m_strQuestTitle = L"�Ҿ ������";
	m_strQuestDesc = L"�� ���������� �����Ⱑ �þ ������ ���� �����.";

	// TODO : ��� ����
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� �ȳ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ��ħ�̾�~");
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
