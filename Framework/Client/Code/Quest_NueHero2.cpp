#include "Quest_NueHero2.h"

CQuest_NueHero2::CQuest_NueHero2()
	:CQuest_Conversation(NPC_CODE::NPC_NUE_HERO, nullptr)
{
	m_strQuestTitle = L"�������� ��ȭ�ϱ�.";
	m_strQuestDesc = L"Ÿ���� ������縦 ȥ�����. �������� �������� ��ȭ�غ���.";

	// TODO :: ������� ��� ����
	
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
	m_eQuestProgress = QUEST_PROGRESS::PROGRESS_END;
	// TODO :: �ƾ� �� ���� ����
}
