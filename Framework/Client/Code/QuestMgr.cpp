#include "QuestMgr.h"
#include "Quest.h"
#include "Quest_Sheep.h"
IMPLEMENT_SINGLETON(CQuestMgr)

CQuestMgr::CQuestMgr()
{
    
}

CQuestMgr::~CQuestMgr()
{

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
        m_vecPlayerQuest[i]->Update_Quest(fTime);
    }
}

void CQuestMgr::Free()
{

}
