#include "Export_Function.h"
#include "Quest_MonkeyCheif2.h"
#include "GameMgr.h"
#include "Item_Hat_Monkey.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_MonkeyCheif3.h"

CQuest_MonkeyCheif2::CQuest_MonkeyCheif2()
	: CQuest_Hunting(NPC_CODE::NPC_CHIEF, nullptr, OBJ_ID::SUN_GOLLEM, 1)
{

	m_strQuestTitle = L"태양의 신전 골렘 처치.";
	m_strQuestDesc = L"쓰레기를 무단투기해 환경을 파괴하는 골렘을 처치하자.";

	// TODO :: 몽키 촌장 대사
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"");

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		m_pRewardItem = CItem_Hat_Monkey::Create(Engine::Get_Device(), pPlayer);
	
	m_iRewardCoin = 1000;
	
}

CQuest_MonkeyCheif2::~CQuest_MonkeyCheif2()
{
}

void CQuest_MonkeyCheif2::Update_Quest(_float& fTimeDelta)
{
	__super::Update_Quest(fTimeDelta);
}

void CQuest_MonkeyCheif2::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_MonkeyCheif2::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;

	CQuest_MonkeyCheif3* pNextQuest = new CQuest_MonkeyCheif3();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer == nullptr || m_pRewardItem == nullptr)
		return;

	CInventoryMgr::GetInstance()->Add_Item(m_pRewardItem);
	pPlayer->Add_Money(m_iRewardCoin);


	
}
