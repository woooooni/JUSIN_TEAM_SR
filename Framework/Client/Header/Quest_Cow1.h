#pragma once
#include "Quest_Hunting.h"
class CQuest_Cow1 : public CQuest_Hunting
{
public:
    explicit CQuest_Cow1();
    virtual ~CQuest_Cow1();

public:
    virtual void Update_Quest(_float& fTimeDelta) override;
    virtual void Accept_Quest() override;
    virtual void Clear_Quest() override;

};

