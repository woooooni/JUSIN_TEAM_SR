#include"Engine_Enum.h"
#include "Export_Function.h"
#include "Quest.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"


CQuest::CQuest(NPC_CODE _eNpcCode, QUEST_TYPE _eQuestType, wstring _strQuestTitle, wstring _strQuestDesc, CItem* _pRewardItem)
	: m_eNpcCode(_eNpcCode)
	, m_eQuestType(_eQuestType)
	, m_strQuestTitle(_strQuestTitle)
	, m_strQuestDesc(_strQuestDesc)
	, m_pRewardItem(_pRewardItem)
	, m_iRewardCoin(0)
	, m_eQuestProgress(QUEST_PROGRESS::BEFORE)
{
	for (_uint i = 0; i < _uint(QUEST_PROGRESS::PROGRESS_END); ++i)
		m_vecNpcDescList[i].reserve(20);
}

void CQuest::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;

	if(nullptr != m_pRewardItem)
		CInventoryMgr::GetInstance()->Add_Item(m_pRewardItem->Clone());


}

