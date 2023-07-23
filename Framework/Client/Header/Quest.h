#pragma once
#include "../Include/stdafx.h"
#include "Item.h"
#include "Engine_Define.h"

class CQuest
{
public:
    explicit CQuest(NPC_CODE _eNpcCode, 
        QUEST_TYPE _eQuestType,
        wstring _strQuestTitle,
        wstring _strQuestDesc,
        vector<wstring>& _vecDescBefore,
        vector<wstring>& _vecDescContinue,
        vector<wstring>& _vecDescComplete,
        CItem* _pRewardItem,
        OBJ_ID _eHuntingID = OBJ_ID::ID_END,
        _uint _iCount = 0);

    virtual ~CQuest() = default;

public:
    const vector<wstring>& Get_NpcDesc(_uint iIdx) 
    { 
        if (iIdx >= m_vecNpcDescList[_uint(m_eNpcCode)].size())
            iIdx = 0;
        
        return m_vecNpcDescList[iIdx];
    }

    QUEST_TYPE Get_QuestType() { return m_eQuestType; }

    QUEST_PROGRESS Get_Quest_Progress() { return m_eQuestProgress; }
    void Set_Quest_Progress(QUEST_PROGRESS _eProgress) { m_eQuestProgress = _eProgress; }
    
    void Set_NpcCode(NPC_CODE _eNpcCode) { m_eNpcCode = _eNpcCode; }
    NPC_CODE Get_NpcCode() { return m_eNpcCode; }

    // For Hunting Quest
    _uint Get_CurrCount() { return m_iCurCount; }
    _uint Get_MaxCount() { return m_iMaxCount; }

public:
    void Update_Quest(_float& fTimeDelta);

public:
    void Accept_Quest();
    void Clear_Quest();

private:
    NPC_CODE m_eNpcCode;
    QUEST_TYPE m_eQuestType;
    QUEST_PROGRESS m_eQuestProgress;
    CItem* m_pRewardItem;

    wstring m_strQuestTitle;
    wstring m_strQuestDesc;

    vector<wstring> m_vecNpcDescList[(_uint)QUEST_PROGRESS::PROGRESS_END];

    // For Hunting Quest
    OBJ_ID m_eHuntingID;

    _uint m_iCurCount;
    _uint m_iMaxCount;
};

