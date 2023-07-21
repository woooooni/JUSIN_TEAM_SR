#pragma once
#include "../Include/stdafx.h"
#include "Engine_Define.h"

class CQuest
{
public:
    explicit CQuest(vector<wstring>& _vecNpcDesc, QUEST_TYPE _eQuestType);
    virtual ~CQuest() = default;

public:
    const wstring& Get_NpcDesc(_uint iIdx) 
    { 
        if (iIdx >= m_vecNpcDesc.size())
            return L""; 
        return m_vecNpcDesc[iIdx];
    }

    QUEST_TYPE Get_QuestType() { return m_eQuestType; }
    QUEST_PROGRESS Get_Quest_Progress() { return m_eQuestProgress; }
    void Set_Quest_Progress(QUEST_PROGRESS _eProgress) { m_eQuestProgress = _eProgress; }
    
    void Set_NpcCode(NPC_CODE _eNpcCode) { m_eNpcCode = _eNpcCode; }
    NPC_CODE Get_NpcCode() { return m_eNpcCode; }
public:
    void Update_Quest(_float& fTimeDelta);


private:
    vector<wstring> m_vecNpcDesc;
    wstring m_strQuestDesc;
    _uint m_iIdx;

    QUEST_TYPE m_eQuestType;
    QUEST_PROGRESS m_eQuestProgress;

    NPC_CODE m_eNpcCode;

};

