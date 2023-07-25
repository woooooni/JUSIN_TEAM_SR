#include "Export_Function.h"
#include "Quest_Hunting.h"

CQuest_Hunting::CQuest_Hunting( NPC_CODE _eNpcCode, 
	CItem* _pRewardItem, 
	OBJ_ID _eHuntingID, 
	_uint _iMaxCount)
	: CQuest(_eNpcCode, QUEST_TYPE::HUNTING, L"", L"", _pRewardItem)
	, m_eHuntingID(_eHuntingID)
	, m_iMaxCount(_iMaxCount)
	, m_iCurCount(0)
{


}

void CQuest_Hunting::Update_Quest(_float& fTimeDelta)
{
	if (QUEST_PROGRESS::CONTINUE == m_eQuestProgress)
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
