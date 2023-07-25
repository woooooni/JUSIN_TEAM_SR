#include "QuestMgr.h"
#include "Quest.h"
IMPLEMENT_SINGLETON(CQuestMgr)

CQuestMgr::CQuestMgr()
{
    
}

CQuestMgr::~CQuestMgr()
{

}

HRESULT CQuestMgr::Ready_QuestMgr()
{
    
    // 퀘스트는 여기서 추가.
    // CQuest* pQuest = new CQuest(NPC_CODE::NPC_SHEEP, QUEST_TYPE::CONVERSATION, )

    return S_OK;
}

void CQuestMgr::Update_QuestMgr(_float& fTimeDelta)
{
    for (_uint i = 0; i < (_uint)NPC_CODE::CODE_END; ++i)
    {
        for (size_t idx = 0; idx < m_vecQuestList[i].size(); ++idx)
            m_vecQuestList[i][idx]->Update_Quest(fTimeDelta);
    }
}

void CQuestMgr::Free()
{
}
