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

void CQuestMgr::Update_QuestMgr(_float& fTimeDelta)
{
    for (_uint i = 0; i < (_uint)NPC_CODE::CODE_END; ++i)
    {
        m_vecPlayerQuest[i]->Update_Quest(fTimeDelta);
    }
}

void CQuestMgr::Free()
{

}
