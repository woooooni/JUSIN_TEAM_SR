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
	void Add_Quest(CQuest* pQuest) 
	{
		if (pQuest == nullptr)
			return;
		m_vecQuestList[_uint(pQuest->Get_NpcCode())].push_back(pQuest);
	}

	void Add_PlayerQuest(CQuest* pQuest) { m_vecPlayerQuest.push_back(pQuest); }

public:
	vector<CQuest*>& Get_QuestVec(NPC_CODE _eCode) { return m_vecQuestList[(_uint)_eCode]; }
	vector<CQuest*>& Get_PlayerQuest() { return m_vecPlayerQuest; }

private:
	vector<CQuest*> m_vecQuestList[(_uint)NPC_CODE::CODE_END];
	vector<CQuest*> m_vecPlayerQuest;

public:
    virtual void Free() override;
};

