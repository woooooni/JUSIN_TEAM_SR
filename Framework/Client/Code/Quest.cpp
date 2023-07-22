#include"Engine_Enum.h"
#include "Export_Function.h"
#include "Quest.h"
#include "InventoryMgr.h"

CQuest::CQuest(NPC_CODE _eNpcCode, 
	QUEST_TYPE _eQuestType, 
	wstring _strQuestTitle,
	wstring _strQuestDesc,
	vector<wstring>& _vecDescBefore, 
	vector<wstring>& _vecDescContinue, 
	vector<wstring>& _vecDescComplete,
	CItem* _pRewardItem,
	OBJ_ID _eHuntingID,
	_uint _iCount)
	: m_eNpcCode(_eNpcCode)
	, m_eQuestType(_eQuestType)
	, m_strQuestTitle(_strQuestTitle)
	, m_strQuestDesc(_strQuestDesc)
	, m_pRewardItem(_pRewardItem)
	, m_eQuestProgress(QUEST_PROGRESS::BEFORE)
	, m_eHuntingID(_eHuntingID)
	, m_iCurCount(0)
	, m_iMaxCount(_iCount)
	
{
	for (_uint i = 0; i < _uint(QUEST_PROGRESS::PROGRESS_END); ++i)
		m_vecNpcDescList[i].reserve(20);

	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)] = _vecDescBefore;
	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)] = _vecDescContinue;
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)] = _vecDescComplete;
}

void CQuest::Update_Quest(_float& fTimeDelta)
{
	if (QUEST_TYPE::HUNTING == m_eQuestType && QUEST_PROGRESS::CONTINUE == m_eQuestProgress)
	{
		const vector<CGameObject*> vecDeleteObj = Engine::Get_DelteObj_Vec(OBJ_TYPE::OBJ_MONSTER);
		for (size_t i = 0; i < vecDeleteObj.size(); ++i)
		{
			if (vecDeleteObj[i]->GetObj_Id() == m_eHuntingID)
			{
				++m_iCurCount;
				if (m_iCurCount >= m_iMaxCount)
				{
					m_eQuestProgress = QUEST_PROGRESS::COMPLETE;
					return;
				}
			}
		}
	}
}

void CQuest::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;

	if(nullptr != m_pRewardItem)
		CInventoryMgr::GetInstance()->Add_Item(m_pRewardItem->Clone());


}

