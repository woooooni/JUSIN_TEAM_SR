#include "QuestMgr.h"
#include "Quest.h"
#include "Quest_Sheep.h"
#include "Export_Utility.h"
IMPLEMENT_SINGLETON(CQuestMgr)

CQuestMgr::CQuestMgr()
{
    
}

CQuestMgr::~CQuestMgr()
{
    Free();
}

HRESULT CQuestMgr::Ready_QuestMgr()
{
    CQuest_Sheep* pQuest = new CQuest_Sheep();
    Add_Quest(pQuest);

    return S_OK;
}

void CQuestMgr::Update_QuestMgr(const _float& fTimeDelta)
{
    _float fTime = fTimeDelta;
    for (size_t i = 0; i < m_vecPlayerQuest.size(); ++i)
    {
        if (m_vecPlayerQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::CONTINUE)
        {
            m_vecPlayerQuest[i]->Update_Quest(fTime);
        }
    }
}

void CQuestMgr::Add_Quest(CQuest* pQuest)
{
    if (pQuest == nullptr)
        return;
    m_vecQuestList[_uint(pQuest->Get_NpcCode())].push_back(pQuest);

}

void CQuestMgr::Free()
{
    for (_uint i = 0; i < _uint(NPC_CODE::CODE_END); ++i)
    {
        for (size_t idx = 0; idx < m_vecQuestList[i].size(); ++idx)
        {
            Safe_Delete(m_vecQuestList[i][idx]);
        }
        m_vecQuestList[i].clear();
    }
}
