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
	// 원숭이 마을로 가게끔 유도함
	m_strQuestTitle = L"원숭이 마을로.";
	m_strQuestDesc = L"깊은 숲에서 아직 쓰레기가 계속 나오는거 같다. 조사해보자.";

	// TODO : 대사 쓰기
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"아직도 숲속에서 고약한 쓰레기 냄새가 난다고 하더군.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"이거 봐. 다섯 놈은 절~~대 아니었어.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"내 생각엔 원숭이 마을 원숭이들이 새벽에 몰~래 갖다 버리는 거 같아.");
	m_vecNpcDescList[(_uint)QUEST_PROGRESS::BEFORE].push_back(L"그놈들 대장을 만나서 그냥 혼쭐을 내줘야 해.");

	m_vecNpcDescList[(_uint)QUEST_PROGRESS::CONTINUE].push_back(L"절~대 5명은 아니었단 말이지..");

//	m_vecNpcDescList[(_uint)QUEST_PROGRESS::COMPLETE].push_back(L""); // 원숭이 촌장에게 말을 걸면 완료되는 퀘스트
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
