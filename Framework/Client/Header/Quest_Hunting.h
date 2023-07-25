#pragma once
#include "Quest.h"
class CQuest_Hunting :
    public CQuest
{
public:
    explicit CQuest_Hunting(NPC_CODE _eNpcCode,
        CItem* _pRewardItem,
        OBJ_ID _eHuntingID,
        _uint _iMaxCount);

    virtual ~CQuest_Hunting() = default;

public:
    _uint Get_CurrCount() { return m_iCurCount; }
    _uint Get_MaxCount() { return m_iMaxCount; }

public:
    virtual void Update_Quest(_float& fTimeDelta) override;

public:
    virtual void Accept_Quest() PURE;
    virtual void Clear_Quest() PURE;

protected:
    // For Hunting Quest
    OBJ_ID m_eHuntingID;

    _uint m_iCurCount;
    _uint m_iMaxCount;
};

