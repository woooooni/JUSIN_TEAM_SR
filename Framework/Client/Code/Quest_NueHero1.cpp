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
	// 보스전 시작 전 대화 내용. 대화 종료 후 바로 보스전이 시작됨.
	m_strQuestTitle = L"타락한 누에용사 처치";
	m_strQuestDesc = L"쓰레기 무단투기 진범인 타락한 누에용사를 처치하자.";
	
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"어떻게 들어 들어온 건가?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"포유류따위가 감히 내 동굴에 들어오다니.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"그렇다. 쓰레기를 선물한건 나다.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"너희들 마을을 우리 곤충들이 지배하는 것이 내 최종 목표다.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"냄새나는 쓰레기를 잔뜩 버려 더럽히면\n동물들이 마을을 떠날 것이 분명하니 말이야.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"그렇게 둘 순 없다고?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"누에용사의 힘을 보아라!");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"말도 안돼!");

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
	// TODO :: 컷씬

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		pPlayer->Add_Money(m_iRewardCoin);

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);


}
