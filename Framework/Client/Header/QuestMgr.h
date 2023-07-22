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

public:
	HRESULT Ready_QuestMgr();
	void Update_QuestMgr(_float& fTimeDelta);

public:
	vector<CQuest*>& Get_QuestVec(NPC_CODE _eCode) { return m_vecQuestList[(_uint)_eCode]; }

private:
	vector<CQuest*> m_vecQuestList[(_uint)NPC_CODE::CODE_END];

public:
    virtual void Free() override;
};

