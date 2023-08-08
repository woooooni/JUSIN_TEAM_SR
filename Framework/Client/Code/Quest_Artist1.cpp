#include "Quest_Artist1.h"
#include "GameMgr.h"
#include "Player_State.h"
#include "QuestMgr.h"
#include "UIMgr.h"
CQuest_Artist1::CQuest_Artist1(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	:CQuest_Conversation(NPC_CODE::NPC_ARTIST, nullptr)
{
	m_strQuestTitle = L"�׸� �׸���";
	m_strQuestDesc = L"������� ���� �������";

	// TODO : ��� ����
	//m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)];
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ������ ������ �����޾Ҿ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� �׸��׸��� ���� �˷�����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�� ���� �� ǥ�ð� ������?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�� ǥ�� �������� �׸��� �׸� �� �־�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�� �� �׷�����?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"� �׸��� �׷���.");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"�Ϳ�! �׸��� ������ �ִ°�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"�����ε� �� ǥ�ø� �߰��ϸ�\n�׳� ����ġ�� ���� �׸��� �׷������� ��!");
	

}

void CQuest_Artist1::Update_Quest(_float& fTimeDelta)
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer != nullptr)
	{
		if (pPlayer->Get_State() == PLAYER_STATE::DRAWING)
			m_eQuestProgress = QUEST_PROGRESS::COMPLETE;
	}
}

void CQuest_Artist1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_Artist1::Clear_Quest()
{
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
}
