#include "Quest_DanceTeacher.h"
#include "GameMgr.h"
#include "Player_State.h"
#include "QuestMgr.h"
#include "UIMgr.h"

CQuest_DanceTeacher::CQuest_DanceTeacher(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	:CQuest_Conversation(NPC_CODE::NPC_DANCETEACHER, nullptr)
{
	m_strQuestTitle = L"���߱�";
	m_strQuestDesc = L"�������� ���� ���ǿ� ���� ���� �纸��.";

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ������, ����, ������, ������ ������.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�� �㼱�� �? ��û����?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"� �ʵ� �ѹ� ���� �纼��?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"� �����غ�!");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"�Ϳ�! ����� �ִ°�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"���� ��ó�� ��̰� ��� ���̶�� �� �� ����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"������ ������ ��ó�� ��ſ� �����ϱ� �ٷ�~");
}

void CQuest_DanceTeacher::Update_Quest(_float& fTimeDelta)
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer != nullptr)
	{
		if (pPlayer->Get_State() == PLAYER_STATE::DANCE)
			m_eQuestProgress = QUEST_PROGRESS::COMPLETE;
	}
}

void CQuest_DanceTeacher::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_DanceTeacher::Clear_Quest()
{
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
}
