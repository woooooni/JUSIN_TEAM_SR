#pragma once
#include "Quest_Conversation.h"
class CQuest_DanceTeacher : public CQuest_Conversation
{

public:
	explicit CQuest_DanceTeacher(NPC_CODE _eNpcCode, CItem* _pRewardItem);
	virtual ~CQuest_DanceTeacher() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

