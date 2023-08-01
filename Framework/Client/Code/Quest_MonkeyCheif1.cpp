#include "Export_Function.h"
#include "Quest_MonkeyCheif1.h"
#include "Quest_MonkeyCheif2.h"
#include "QuestMgr.h"
#include "UIMgr.h"

CQuest_MonkeyCheif1::CQuest_MonkeyCheif1()
	:CQuest_Conversation(NPC_CODE::NPC_CHIEF, nullptr)
{
	// ������ ���忡�� ���� �ɸ� ����Ʈ �Ϸ�.
	m_strQuestTitle = L"������ ���� ����";
	m_strQuestDesc = L"�����̵��� �ǽɽ�����. ������ ��������.";

	// TODO : ��� ����
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ ������ ��.. �����ʱ���?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�츮 ���� �ֹε��� ���� ������ �����⸦ �����ٰ�?");

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
