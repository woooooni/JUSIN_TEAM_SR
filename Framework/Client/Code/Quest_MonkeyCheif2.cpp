#include "Export_Function.h"
#include "Quest_MonkeyCheif2.h"
#include "GameMgr.h"
#include "Item_Hat_Monkey.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_MonkeyCheif3.h"
#include "UIMgr.h"
#include "EventMgr.h"
#include "Item_Hat_Turtle.h"
#include "SunGollem.h"

CQuest_MonkeyCheif2::CQuest_MonkeyCheif2()
	: CQuest_Hunting(NPC_CODE::NPC_CHIEF, nullptr, OBJ_ID::SUN_GOLLEM, 1)
{
	//태양의 신전 안 골렘을 처치하도록 한다. 보스전 유도
	m_strQuestTitle = L"태양의 신전 골렘 처치.";
	m_strQuestDesc = L"쓰레기를 무단투기해 환경을 파괴하는 골렘을 처치하자.";

	// TODO :: 몽키 촌장 대사
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"뭔가 오해가 있는 것 같다.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"쓰레기로 고통받고 있는 건 우리 마을이라고!");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"오리너구리! 네 마을에서 쓰레기를 내버린 것이 아니라면,");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"아무래도 저 신전이 가장 의심스럽다.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"매일같이 악취가 진동하고 밤마다 괴음이 들리는데...");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"오늘 한 번 가서 살펴보는 것도 좋을 것 같군.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"이 모자가 도움이 될걸세.");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"왜 아직도 여기서 이러고 있나?");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"골렘 신전 안에 쓰레기 같은 건 없었다고?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"어찌 되었든 골렘이 우리 마을을 떠났다니");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"덕분에 앞으로 편히 잘 수 있겠어.");

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
	const vector<CGameObject*>& vecMonster = Engine::Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec();
	for (auto iter : vecMonster)
	{
		if (iter->GetObj_Id() == OBJ_ID::SUN_GOLLEM)
		{
			CSunGollem* pGolem = dynamic_cast<CSunGollem*>(iter);
			NULL_CHECK_RETURN(pGolem);

			if (pGolem->Get_State() == SUNGOLEM_STATE::DIE)
			{
				m_eQuestProgress = QUEST_PROGRESS::COMPLETE;
				return;
			}
		}
	}
	
}

void CQuest_MonkeyCheif2::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Reserve_Active(true);

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if(pPlayer != nullptr)
		CInventoryMgr::GetInstance()->Add_Item(CItem_Hat_Turtle::Create(Engine::Get_Device(), pPlayer));
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

	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}
