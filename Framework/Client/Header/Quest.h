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
        CItem* _pRewardItem);

    virtual ~CQuest() = default;

public:
    const vector<wstring>& Get_NpcDesc() { return m_vecNpcDescList[_uint(m_eQuestProgress)]; }

    QUEST_TYPE Get_QuestType() { return m_eQuestType; }

    QUEST_PROGRESS Get_Quest_Progress() { return m_eQuestProgress; }
    void Set_Quest_Progress(QUEST_PROGRESS _eProgress) { m_eQuestProgress = _eProgress; }
    
    void Set_NpcCode(NPC_CODE _eNpcCode) { m_eNpcCode = _eNpcCode; }
    NPC_CODE Get_NpcCode() { return m_eNpcCode; }

    CItem* Get_Reward() { return m_pRewardItem; }

public:
    virtual void Update_Quest(_float& fTimeDelta) PURE;

public:
    virtual void Accept_Quest() PURE;
    virtual void Clear_Quest() PURE;

protected:
    wstring m_strQuestTitle;
    wstring m_strQuestDesc;
    vector<wstring> m_vecNpcDescList[(_uint)QUEST_PROGRESS::PROGRESS_END];

protected:
    NPC_CODE m_eNpcCode;
    QUEST_TYPE m_eQuestType;
    QUEST_PROGRESS m_eQuestProgress;
    
    CItem* m_pRewardItem;
    _uint m_iRewardCoin;


};

