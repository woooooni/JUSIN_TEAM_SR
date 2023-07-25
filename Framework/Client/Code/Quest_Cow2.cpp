#include "Export_Function.h"
#include "Quest_Cow2.h"
#include "Quest_MonkeyCheif1.h"
#include "QuestMgr.h"

CQuest_Cow2::CQuest_Cow2()
	:CQuest_Conversation(NPC_CODE::NPC_COW, nullptr)
{
	m_strQuestTitle = L"동굴 조사";
	m_strQuestDesc = L"마을 근처 동굴에서 계속 쓰레기가 나오는거 같다. 조사해보자.";

	// TODO : 대사 쓰기
	/*m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"오구 안녕?");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"좋은 아침이야~");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"어휴.. 누가 이렇게 쓰레기를 막 버린거야?");*/
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
