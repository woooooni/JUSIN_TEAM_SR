#include "Quest_Sheep.h"
#include "QuestMgr.h"
#include "Quest_Cow1.h"
#include "UIMgr.h"
CQuest_Sheep::CQuest_Sheep()
	:CQuest_Conversation(NPC_CODE::NPC_SHEEP, nullptr)
{
	m_strQuestTitle = L"�Ҿ ������";
	m_strQuestSub = L"ö���� �� ������ ã�ư���";
	m_strQuestDesc = L"�ñ��� �� �� ���� �Ʊ����!\n�� �������� �� ���� �������� �����.";

	// TODO : ��� ����
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� �ȳ�? ���� ��ħ�̾�~");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"��� ������� �Ŵ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� �ϴ� ���� ���� �ܰ���\n��ü�Ҹ��� �������� ������ϴٰ� �ϴ���..");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ö���� �� ��������\n���� �ܰ����� �̻��� �� �ôٰ� �ϴ���.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�ƹ�ư, �ʹ� ���� �ð����� ���ƴٴ��� ����!\n�ƹ��� ���󰡸� ū�� ����~");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"�� �������� �� �ô��� �ñ����� �ʴ�?");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L"�츮 ���� �����⸦ ������ �༮���� �ִٰ�?");
}

void CQuest_Sheep::Update_Quest(_float& fTimeDelta)
{
}

void CQuest_Sheep::Accept_Quest()
{
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
	Clear_Quest();

	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Reserve_Active(true);

	CUIMgr::GetInstance()->Get_QuestList()->Set_Title(m_strQuestSub);
	CUIMgr::GetInstance()->Get_QuestList()->Set_Active(true);
}

void CQuest_Sheep::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_Cow1* pQuest = new CQuest_Cow1();
	CQuestMgr::GetInstance()->Add_Quest(pQuest);
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}
