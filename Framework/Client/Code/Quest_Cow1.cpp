#include "Export_Function.h"
#include "Quest_Cow1.h"
#include "GameMgr.h"
#include "Item_Hat_Light.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"
#include "Quest_Cow2.h"

CQuest_Cow1::CQuest_Cow1()
	: CQuest_Hunting(NPC_CODE::NPC_COW, nullptr, OBJ_ID::TRASH_SLIME, 5)
{

	m_strQuestTitle = L"������ û��";
	//m_strQuestDesc = L"���� ����� �����⸦ û������.";
	m_strQuestDesc = L"�����⸦ ���� �����ϴ� �Ǵ���� ȥ������!";

	// TODO :: �Ҿ����� ���
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���� �ȳ�? ���� �ɺθ� �Դ�?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�ƴ϶��? �׷� ��¾ ���̴�?"); // �̰� ���� �� ��
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"����... ���� �׳� ������ �� ���� �ñ��ϴٰ�?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�� ���� ���ʿ��� �̻��� �༮���� ���� ���� ��������");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�� �̰������������ ������ ���� ��������\n�ܶ� ������� ������ܴ�.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���� �ճ⿡~\n�� �����Ϳ��� 17 �� 1�� �ο��� �̱� ���� �־��!");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�׷��� ȥ���ַ��� �� �޷����µ�...");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"���� �����⸦ ������ �༮���� 18������ ����.");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::BEFORE)].push_back(L"�� �� ����� ���� ȥ���� ���ִ� �ǵ� ���̾�.");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"�����? ���ϰ����̶��?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::CONTINUE)].push_back(L"�ƴ϶���. �и� ���������̾���. �ٽ� ���� �����.\n�������� �ٺ��� �̸�");

	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"�ټ� ���̾��ٴ�..."); // trash big 5���� ���
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"���� ��� � �����ٰ� ����?\n�뵷 �� �ʿ��ϴ�?");
	m_vecNpcDescList[_uint(QUEST_PROGRESS::COMPLETE)].push_back(L"�̰� �ް�! ���� ���� �̾߱�� �� ���� �ɷ� ���ַ�.");

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer != nullptr)
		m_pRewardItem = CItem_Hat_Light::Create(Engine::Get_Device(), pPlayer);
	
	m_iRewardCoin = 100;
	
}

CQuest_Cow1::~CQuest_Cow1()
{
}

void CQuest_Cow1::Update_Quest(_float& fTimeDelta)
{
	__super::Update_Quest(fTimeDelta);
}

void CQuest_Cow1::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_Cow1::Clear_Quest()
{
	CQuest_Cow2* pNextQuest = new CQuest_Cow2();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();

	if (pPlayer == nullptr || m_pRewardItem == nullptr)
		return;

	CInventoryMgr::GetInstance()->Add_Item(m_pRewardItem);
	pPlayer->Add_Money(m_iRewardCoin);


}
