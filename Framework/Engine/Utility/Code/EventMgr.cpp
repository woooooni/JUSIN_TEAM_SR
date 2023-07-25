#include "EventMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CEventMgr)
CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{
}



HRESULT CEventMgr::Ready_EventMgr()
{
	for (_uint i = 0; i < _uint(OBJ_TYPE::OBJ_END); ++i)
		m_vecDeleteObj[i].reserve(100);

	return S_OK;
}

void CEventMgr::Update_EventMgr(const _float& fTimeDelta)
{
	for (_uint i = 0; i < _uint(OBJ_TYPE::OBJ_END); ++i)
	{
		for (size_t idx = 0; idx < m_vecDeleteObj[i].size(); ++idx)
		{
			if (nullptr != m_vecDeleteObj[i][idx])
				Safe_Release(m_vecDeleteObj[i][idx]);
		}
		m_vecDeleteObj[i].clear();
	}

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
		Excute(m_vecEvent[i]);

	m_vecEvent.clear();
	
}

void CEventMgr::AddEvent(const EVENT_DESC _tDesc)
{
	m_vecEvent.push_back(_tDesc);
}

void CEventMgr::Excute(const EVENT_DESC& _tDesc)
{
	switch (_tDesc.eEvent)
	{
	//case EVENT_TYPE::CHANGE_SCENE:
	//	// lParam : SCENE_TYPE
	//	CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_tDesc.lParam);
	//	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		CGameObject* pObj = (CGameObject*)_tDesc.lParam;
		if (nullptr != pObj)
			m_vecDeleteObj[(_uint)pObj->GetObj_Type()].push_back(pObj);
	}
		break;
	}
}

void CEventMgr::Free()
{

}
