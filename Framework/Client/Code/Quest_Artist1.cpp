#include "Quest_Artist1.h"
#include "GameMgr.h"
#include "Player_State.h"
#include "QuestMgr.h"
#include "UIMgr.h"
CQuest_Artist1::CQuest_Artist1(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	:CQuest_Conversation(NPC_CODE::NPC_ARTIST, nullptr)
{
	m_strQuestTitle = L"그림 그리기";
	m_strQuestDesc = L"삼원색에 대해 배워보자";

	// TODO : 대사 쓰기
	//m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)];
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"나의 현란한 붓질에 감동받았어?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"내가 그림그리는 법을 알려주지.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"내 옆에 이 표시가 보이지?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"이 표시 위에서는 그림을 그릴 수 있어.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"힌 번 그려볼래?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"어서 그림을 그려봐.");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"와우! 그림에 소질이 있는걸?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"앞으로도 이 표시를 발견하면\n그냥 지나치지 말고 그림을 그려보도록 해!");
	

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
