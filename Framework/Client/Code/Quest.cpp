#include"Engine_Enum.h"
#include "Export_Function.h"
#include "Quest.h"

CQuest::CQuest(vector<wstring>& _vecNpcDesc, QUEST_TYPE _eQuestType)
	: m_eQuestType(_eQuestType)
	, m_eQuestProgress(QUEST_PROGRESS::BEFORE)
{
	m_vecNpcDesc.reserve(_vecNpcDesc.size());
	m_vecNpcDesc = _vecNpcDesc;
}

void CQuest::Update_Quest(_float& fTimeDelta)
{
	
}

