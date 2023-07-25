#pragma once
#include "Quest_Conversation.h"
class CQuest_Cow2 : public CQuest_Conversation
{

public:
	explicit CQuest_Cow2();
	virtual ~CQuest_Cow2() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

