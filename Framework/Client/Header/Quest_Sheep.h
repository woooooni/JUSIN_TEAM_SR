#pragma once
#include "Quest_Conversation.h"
class CQuest_Sheep : public CQuest_Conversation
{

public:
	explicit CQuest_Sheep();
	virtual ~CQuest_Sheep() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

