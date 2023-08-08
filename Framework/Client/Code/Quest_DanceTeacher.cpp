#include "Quest_DanceTeacher.h"
#include "GameMgr.h"
#include "Player_State.h"
#include "QuestMgr.h"
#include "UIMgr.h"

CQuest_DanceTeacher::CQuest_DanceTeacher(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	:CQuest_Conversation(NPC_CODE::NPC_DANCETEACHER, nullptr)
{
	m_strQuestTitle = L"춤추기";
	m_strQuestDesc = L"선생님을 따라 음악에 맞춰 춤을 춰보자.";

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"원투 차차차, 투투, 차차차, 쓰리포 차차차.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"내 춤선이 어때? 엄청나지?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"어때 너도 한번 따라 춰볼래?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"어서 따라해봐!");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"와우! 재능이 있는걸?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"춤은 너처럼 즐겁게 춰야 춤이라고 할 수 있지.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"앞으로 원숭이 댄스처럼 즐거운 모험하길 바래~");
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
