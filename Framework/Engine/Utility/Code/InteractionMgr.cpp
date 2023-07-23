#include "InteractionMgr.h"
#include "..\..\Header\Export_Utility.h"

USING(Engine)

IMPLEMENT_SINGLETON(CInteractionMgr)

Engine::CInteractionMgr::CInteractionMgr()
{
	ZeroMemory(m_bEventSwitch, sizeof(bool) * 1000);
	m_vecResetList.resize(100);
}

Engine::CInteractionMgr::~CInteractionMgr()
{
	Free();
}

void CInteractionMgr::Set_Event()
{
}

void CInteractionMgr::Free()
{
	for (auto& iter : m_mapEvents)
	{
		Safe_Delete(iter.second);
	}
	m_mapEvents.clear();
}




