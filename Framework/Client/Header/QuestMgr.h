#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Quest.h"

class CQuestMgr :
    public CBase
{
  DECLARE_SINGLETON(CQuestMgr)

private:
	explicit CQuestMgr();
	virtual ~CQuestMgr();

private:
	vector<CQuest*> m_vecQuestList[(_uint)NPC_CODE::CODE_END];

public:
    virtual void Free() override;
};

