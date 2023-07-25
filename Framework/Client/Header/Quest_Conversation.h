#pragma once
#include "Quest.h"

class CQuest_Conversation : public CQuest
{
public:
    explicit CQuest_Conversation(NPC_CODE _eNpcCode, CItem* _pRewardItem);

    virtual ~CQuest_Conversation() = default;

public:
    virtual void Update_Quest(_float& fTimeDelta) PURE;

public:
    virtual void Accept_Quest() PURE;
    virtual void Clear_Quest() PURE;

};

