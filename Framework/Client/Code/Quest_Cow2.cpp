#include "Export_Function.h"
#include "Quest_Cow2.h"
#include "Quest_MonkeyCheif1.h"
#include "QuestMgr.h"

CQuest_Cow2::CQuest_Cow2()
	:CQuest_Conversation(NPC_CODE::NPC_COW, nullptr)
{
	m_strQuestTitle = L"���� ����";
	m_strQuestDesc = L"���� ��ó �������� ��� �����Ⱑ �����°� ����. �����غ���.";

	// TODO : ��� ����
	/*m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� �ȳ�?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"���� ��ħ�̾�~");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"����.. ���� �̷��� �����⸦ �� �����ž�?");*/
}

void CQuest_Cow2::Update_Quest(_float& fTimeDelta)
{
	CScene* pScene = Engine::GetCurrScene();
	if (pScene == nullptr)
		return;

	if (pScene->Get_SceneType() == SCENE_TYPE::MONKEY_VILLAGE)
	{
		Clear_Quest();
	}
}

void CQuest_Cow2::Accept_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::CONTINUE;
	CQuestMgr::GetInstance()->Add_PlayerQuest(this);
}

void CQuest_Cow2::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_MonkeyCheif1* pNextQuest = new CQuest_MonkeyCheif1();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);
}
