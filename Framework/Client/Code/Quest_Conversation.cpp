#include "Quest_Conversation.h"

CQuest_Conversation::CQuest_Conversation(NPC_CODE _eNpcCode, CItem* _pRewardItem)
	: CQuest(_eNpcCode, QUEST_TYPE::CONVERSATION, L"", L"", _pRewardItem)
{

}
