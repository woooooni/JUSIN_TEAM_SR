#pragma once
#include "Quest_Conversation.h"
class CQuest_MonkeyCheif3 : public CQuest_Conversation
{

public:
	explicit CQuest_MonkeyCheif3();
	virtual ~CQuest_MonkeyCheif3() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

