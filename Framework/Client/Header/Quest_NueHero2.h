#pragma once
#include "Quest_Conversation.h"
class CQuest_NueHero2 : public CQuest_Conversation
{

public:
	explicit CQuest_NueHero2();
	virtual ~CQuest_NueHero2() = default;

public:
	virtual void Update_Quest(_float& fTimeDelta) override;
	virtual void Accept_Quest() override;
	virtual void Clear_Quest() override;
};

