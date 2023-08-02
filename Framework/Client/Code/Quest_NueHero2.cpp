#include "UIMgr.h"
#include "Quest_NueHero2.h"
#include "GameMgr.h"

CQuest_NueHero2::CQuest_NueHero2()
	:CQuest_Conversation(NPC_CODE::NPC_NUE_HERO, nullptr)
{
	// ���� �������
	m_strQuestTitle = L"�������� ��ȭ�ϱ�.";
	//m_strQuestSub = L""; // ����
	m_strQuestDesc = L"Ÿ���� ������縦 ȥ�����. �������� �������� ��ȭ�غ���.";

	// TODO :: ������� ��� ����
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��� �Ȱ���?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ �԰� ������ �ڷ� ����� ���� �ʾ�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���忡�� ����� ������ ������\n�������� �Ĺ��� �ڶ󳭴� �̾߱� �����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�ƹ����� �츮 ���� �Ĺ��鿡 ���� �� ������. ������ ��¼��...");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�װ� �� ������ �� ������! �� ������ ��� ����?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ ���� ������ �����⸦ ������ ���� �����ž�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ ����! �ʴ� �츮 ������ �����̾�!");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���п� ������ �����Ⱑ �������!");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ �������� ������ �ڿ��� ���ƿԴ�. \n �ʴ� �� ������ �����̾�");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�ƺ��� �װ� �ڶ������ܴ�.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ������ �������ϱ� ����ؾ߰���?");



	// ���۰ŷ��� ���� �����ھ�� ^^

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"");
	
}

void CQuest_NueHero2::Update_Quest(_float& fTimeDelta)
{
	
}

void CQuest_NueHero2::Accept_Quest()
{
	Clear_Quest();
}

void CQuest_NueHero2::Clear_Quest()
{
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CGameMgr::GetInstance()->Get_Player()->Change_State(PLAYER_STATE::DOWN);

	// TODO :: �ƾ� �� ���� ����
}
