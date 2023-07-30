#include "Export_Function.h"
#include "Quest_MonkeyCheif2.h"
#include "GameMgr.h"
#include "Item_Hat_Monkey.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_MonkeyCheif3.h"
#include "UIMgr.h"
#include "EventMgr.h"

CQuest_MonkeyCheif2::CQuest_MonkeyCheif2()
	: CQuest_Hunting(NPC_CODE::NPC_CHIEF, nullptr, OBJ_ID::SUN_GOLLEM, 1)
{
	//�¾��� ���� �� ���� óġ�ϵ��� �Ѵ�. ������ ����
	m_strQuestTitle = L"�¾��� ���� �� óġ.";
	m_strQuestDesc = L"�����⸦ ���������� ȯ���� �ı��ϴ� ���� óġ����.";

	// TODO :: ��Ű ���� ���
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���� ���ذ� �ִ� �� ����.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"������� ����ް� �ִ� �� �츮 �����̶��!");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�����ʱ���! �� �������� �����⸦ ������ ���� �ƴ϶��,");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�ƹ����� �� ������ ���� �ǽɽ�����.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���ϰ��� ���밡 �����ϰ� �㸶�� ������ �鸮�µ�...");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���� �� �� ���� ���캸�� �͵� ���� �� ����.");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"�� ������ ���⼭ �̷��� �ֳ�?");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"�� ���� �ȿ� ������ ���� �� �����ٰ�?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"���� �Ǿ��� ���� �츮 ������ �����ٴ�");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"���п� ������ ���� �� �� �ְھ�.");

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

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Active(true);
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
