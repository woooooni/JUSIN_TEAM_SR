#include "Export_Function.h"
#include "Quest_NueHero1.h"
#include "GameMgr.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_NueHero2.h"
#include "UIMgr.h"
#include "Npc_NueHero.h"
#include "SilkWorm.h"

CQuest_NueHero1::CQuest_NueHero1()
	: CQuest_Hunting(NPC_CODE::NPC_NUE_HERO, nullptr, OBJ_ID::SILK_WORM, 1)
{
	// ������ ���� �� ��ȭ ����. ��ȭ ���� �� �ٷ� �������� ���۵�.
	m_strQuestTitle = L"Ÿ���� ������� óġ";
	m_strQuestDesc = L"������ �������� ������ Ÿ���� ������縦 óġ����.";
	
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��� ��� ���� �ǰ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������������ ���� �� ������ �����ٴ�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�׷���. �����⸦ �����Ѱ� ����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"����� ������ �츮 ������� �����ϴ� ���� �� ���� ��ǥ��.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�������� �����⸦ �ܶ� ���� ��������\n�������� ������ ���� ���� �и��ϴ� ���̾�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�׷��� �� �� ���ٰ�?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"��������� ���� ���ƶ�!");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"���� �ȵ�!");

	m_iRewardCoin = 100000;
}

CQuest_NueHero1::~CQuest_NueHero1()
{

}

void CQuest_NueHero1::Update_Quest(_float& fTimeDelta)
{
	__super::Update_Quest(fTimeDelta);

	if (m_eQuestProgress == QUEST_PROGRESS::CONTINUE)
		Clear_Quest();
}

void CQuest_NueHero1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_NueHero1::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_NueHero2* pNextQuest = new CQuest_NueHero2();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);

	dynamic_cast<CNpc_NueHero*>(Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Nue_NPC"))->Set_Summoned();
	CSilkWorm* pSilkWorm = CSilkWorm::Create(Engine::Get_Device());
	_vec3 vPos = _vec3(53.f, 0.5f, 26.f);
	pSilkWorm->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	Get_Layer(LAYER_TYPE::MONSTER)->Add_GameObject(L"SilkWorm", pSilkWorm);
	// TODO :: �ƾ�

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		pPlayer->Add_Money(m_iRewardCoin);

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);


}
