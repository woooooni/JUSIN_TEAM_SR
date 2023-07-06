#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

typedef struct tagEvent
{
	_uint iEventNum;
	list<_uint>		lStartKey;
	list<_uint>		lEndKey;
	list<CGameObject*> lSubscribers;
} EVENT;

class CEventMgr :  public CBase
{
	DECLARE_SINGLETON(CEventMgr)

	explicit		CEventMgr();
	~CEventMgr();


public:
	HRESULT		Add_Event(EVENT* pEvent)
	{
		auto iter = m_mapEvents.find(pEvent->iEventNum);

		if (iter != m_mapEvents.end())
			return E_FAIL;

		m_mapEvents.insert({ pEvent->iEventNum, pEvent });
	}

	void		Set_Event();

	HRESULT		Add_Subscribe(_uint pEventKey, CGameObject* pSubscriber)
	{
		auto iter = m_mapEvents.find(pEventKey);

		if (iter == m_mapEvents.end())
			return E_FAIL;

		iter->second->lSubscribers.push_back(pSubscriber);

		return S_OK;
	}

	HRESULT		Check_Event_Start(const _uint& pCheckNum)
	{
		auto iter = m_mapEvents.find(pCheckNum);
		if (iter == m_mapEvents.end())
			return E_FAIL;

		for (auto& iterB : iter->second->lStartKey)
		{
			if (!m_bEventSwitch[iterB])
				return E_FAIL;
		}

		Start_Event(iter->second);
		return S_OK;

	}

	void			Check_Event_End()
	{
		for (auto iter = m_listCurActiveEvents.begin(); iter != m_listCurActiveEvents.end();)
		{
			if (Check_EventSwitch((*iter)->lEndKey))
			{
				for (auto& iterB : (*iter)->lSubscribers)
				{
					//이벤트 종료신호 보내기
				}
				iter = m_listCurActiveEvents.erase(iter);
			}
			else
				++iter;
		}
	}

	void		Start_Event(EVENT* pEvent)
	{
		for (auto& iter : pEvent->lSubscribers)
		{
			//이벤트 시작신호 보내기
		}
		if (!pEvent->lEndKey.empty())
			m_listCurActiveEvents.push_back(pEvent);

		m_bEventSwitch[pEvent->iEventNum] = true;
		Check_Event_End();

	}

	bool		Check_EventSwitch(const list<_uint>& pList)
	{
		for (auto iter : pList)
		{
			if (!m_bEventSwitch[iter])
				return false;
		}
		return true;
	}


private:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

	list<EVENT*> m_listCurActiveEvents;

	map<_uint, EVENT*> m_mapEvents;

	_bool m_bEventSwitch[1000];

};

