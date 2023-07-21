#include "EventMgr.h"
#include "..\..\Header\Export_Utility.h"

USING(Engine)

IMPLEMENT_SINGLETON(CEventMgr)

Engine::CEventMgr::CEventMgr()
{
	ZeroMemory(m_bEventSwitch, sizeof(bool) * 1000);
	
	m_vecResetList.resize(100);
}

Engine::CEventMgr::~CEventMgr()
{
	Free();
}

void CEventMgr::Set_Event()
{
}

void CEventMgr::Free()
{
	for (auto& iter : m_mapEvents)
	{
		Safe_Delete(iter.second);
	}
	m_mapEvents.clear();
}




