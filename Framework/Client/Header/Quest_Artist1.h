#pragma once
#include "Quest_Conversation.h"
class CQuest_Artist1 : public CQuest_Conversation
{

public:
	explicit CQuest_Artist1(NPC_CODE _eNpcCode, CItem* _pRewardItem);
	virtual ~CQuest_Artist1() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

