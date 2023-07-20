#pragma once
#include "Base.h"
#include "Engine_Define.h"

class CQuestMgr :
    public CBase
{
  DECLARE_SINGLETON(CQuestMgr)
private:
	explicit CQuestMgr();
	virtual ~CQuestMgr();

    virtual void Free() override;
};

