#include "Export_Function.h"
#include "Quest_NueHero1.h"
#include "GameMgr.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_NueHero2.h"
#include "UIMgr.h"
#include "Npc_NueHero.h"
#include "SilkWorm.h"
#include "CutSceneMgr.h"


CQuest_NueHero1::CQuest_NueHero1()
	: CQuest_Hunting(NPC_CODE::NPC_NUE_HERO, nullptr, OBJ_ID::SILK_WORM, 1)
{
	// ������ ���� �� ��ȭ ����. ��ȭ ���� �� �ٷ� �������� ���۵�.
	m_strQuestTitle = L"Ÿ���� ������� óġ";
	//m_strQuestSub = L""; // ����
	m_strQuestDesc = L"������ �������� ������ Ÿ���� ������縦 óġ����.";
	
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��� ��� ���� �ǰ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������������ ���� �� ������ �����ٴ�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�׷���. �����⸦ �����Ѱ� ����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"����� ������ �츮 ������� �����ϴ� ���� �� ���� ��ǥ��.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�������� �����⸦ �ܶ� ���� ��������\n�������� ������ ���� ���� �и��ϴ� ���̾�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�׷��� �� �� ���ٰ�?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"��������� ���� ���ƶ�!");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"���� �ȵ�!");

}

CQuest_NueHero1::~CQuest_NueHero1()
{

}

void CQuest_NueHero1::Update_Quest(_float& fTimeDelta)
{
	__super::Update_Quest(fTimeDelta);
	if (m_eQuestProgress == QUEST_PROGRESS::CONTINUE)
	{
		CUIMgr::GetInstance()->Get_QuestList()->Set_Shown(false);
		CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
		Clear_Quest();
	}
}

void CQuest_NueHero1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}

void CQuest_NueHero1::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_NueHero2* pNextQuest = new CQuest_NueHero2;
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);

	CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::BOSS_NUEHERO_INTRO);
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		pPlayer->Add_Money(m_iRewardCoin);

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);


}
