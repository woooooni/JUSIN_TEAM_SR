#include "Export_Function.h"
#include "Quest_Cow2.h"
#include "Quest_MonkeyCheif1.h"
#include "Quest_Artist1.h"
#include "QuestMgr.h"
#include "UIMgr.h"
#include "UI_NewQuest.h"

CQuest_Cow2::CQuest_Cow2()
	:CQuest_Conversation(NPC_CODE::NPC_COW, nullptr)
{
	// ������ ������ ���Բ� ������
	m_strQuestTitle = L"������ ������.";
	m_strQuestDesc = L"���� ������ ���� �����Ⱑ ��� �����°� ����. �����غ���.";

	// TODO : ��� ����
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"������ ���ӿ��� ����� ������ ������ ���ٰ� �ϴ���.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�̰� ��. �ټ� ���� ��~~�� �ƴϾ���.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�� ������ ������ ���� �����̵��� ������ ��~�� ���� ������ �� ����.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"�׳�� ������ ������ �׳� ȥ���� ����� ��.");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"��~�� 5���� �ƴϾ��� ������..");

//	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L""); // ������ ���忡�� ���� �ɸ� �Ϸ�Ǵ� ����Ʈ
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
	CQuestMgr::GetInstance()->Add_Quest(new CQuest_Artist1(NPC_CODE::NPC_ARTIST, nullptr));
	
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Title(m_strQuestTitle);
	CUIMgr::GetInstance()->Get_NewQuestUI()->Set_Desc(m_strQuestDesc);

	CUIMgr::GetInstance()->Get_NewQuestUI()->Reserve_Active(true);
}

void CQuest_Cow2::Clear_Quest()
{
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	CQuest_MonkeyCheif1* pNextQuest = new CQuest_MonkeyCheif1();
	CQuestMgr::GetInstance()->Add_Quest(pNextQuest);
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
}
