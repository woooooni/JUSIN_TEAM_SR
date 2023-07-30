#pragma once
#ifndef EventMgr_h__
#define	EventMgr_h__
#endif // !CollisionMgr_h__



#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"


BEGIN(Engine)
typedef struct tagEvent
{
	_uint iEventNum;
	list<_uint>		lStartKey;
	list<_uint>		lEndKey;
	list<CGameObject*> lSubscribers;
	bool		m_bIsCanReset = false;
	_bool		m_bIsCheckUpdate = false;
} EVENT;



class ENGINE_DLL CInteractionMgr
{
	DECLARE_SINGLETON(CInteractionMgr)

private:
	explicit		CInteractionMgr();
	~CInteractionMgr();


public:
	inline HRESULT		Add_Event(EVENT* pEvent)
	{
		auto iter = m_mapEvents.find(pEvent->iEventNum);

		if (iter != m_mapEvents.end())
			return E_FAIL;

		m_mapEvents.insert({ pEvent->iEventNum, pEvent });

		if (pEvent->m_bIsCheckUpdate)
			m_listCurActiveEvents.push_back(pEvent);
	}

	void		Set_Event();

	inline HRESULT		Add_Subscribe(_uint pEventKey, CGameObject* pSubscriber)
	{
		auto iter = m_mapEvents.find(pEventKey);

		if (iter == m_mapEvents.end())
			return E_FAIL;

		iter->second->lSubscribers.push_back(pSubscriber);

		return S_OK;
	}

	inline HRESULT		Check_Event_Start(const _uint& pCheckNum)
	{
		if (pCheckNum == 0)
			return E_FAIL;

		auto iter = m_mapEvents.find(pCheckNum);
		if (iter == m_mapEvents.end())
			return E_FAIL;
		if (!iter->second->lStartKey.empty())
		{
			for (auto& iterB : iter->second->lStartKey)
			{
				if (!m_bEventSwitch[iterB])
					return E_FAIL;
			}
		}

		if (m_bEventSwitch[pCheckNum] && !iter->second->m_bIsCanReset)
			return S_OK;

		Start_Event(iter->second);

		return S_OK;
	}

private:
	void			Check_Event_End()
	{
		for (auto iter = m_listCurActiveEvents.begin(); iter != m_listCurActiveEvents.end();)
		{
			if (Check_EventSwitch((*iter)->lEndKey))
			{
				for (auto& iterB : (*iter)->lSubscribers)
				{
					iterB->Event_End((*iter)->iEventNum);
					if((*iter)->m_bIsCanReset)
						m_bEventSwitch[(*iter)->iEventNum] = !m_bEventSwitch[(*iter)->iEventNum];
					
				}
				if ((*iter)->m_bIsCheckUpdate && !m_bEventSwitch[(*iter)->iEventNum])
				{
					m_bEventSwitch[(*iter)->iEventNum] = !m_bEventSwitch[(*iter)->iEventNum];
				}

				if (!(*iter)->m_bIsCanReset)
					iter = m_listCurActiveEvents.erase(iter);
				else
					++iter;
			}
			else
				++iter;
		}
	}

	void		Start_Event(EVENT* pEvent)
	{
		for (auto iter = pEvent->lSubscribers.begin(); iter != pEvent->lSubscribers.end();)
		{
			if ((*iter) != nullptr && (*iter)->Is_Active())
			{
				(*iter)->Event_Start(pEvent->iEventNum);
				++iter;
			}
			else
				iter = pEvent->lSubscribers.erase(iter);
		}

		if (!pEvent->lEndKey.empty())
			m_listCurActiveEvents.push_back(pEvent);

		m_bEventSwitch[pEvent->iEventNum] = !m_bEventSwitch[pEvent->iEventNum];


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

public:


	void		Add_Reset(const _uint& resetIndex, const _uint& eventKey)
	{
		m_vecResetList[resetIndex].push_back(eventKey);
	}
	void		Reset(const _uint& resetIndex)
	{
		for (auto& iter : m_vecResetList[resetIndex])
		{
			if (m_mapEvents.find(iter) == m_mapEvents.end())
				continue;

			for (auto& tmp : m_mapEvents.find(iter)->second->lSubscribers)
			{
				tmp->Reset_Event();
			}
			m_bEventSwitch[m_mapEvents.find(iter)->second->iEventNum] = false;

			if (m_mapEvents.find(iter)->second->m_bIsCheckUpdate)
			{
				m_listCurActiveEvents.push_back(m_mapEvents.find(iter)->second);
			}
		}

	}

	void		Add_Reset(const _uint& resetIndex, const _uint& minIndex, const _uint& maxIndex)
	{
		if (minIndex < 0 ||  maxIndex < 0  || minIndex >= maxIndex)
		{
			return;
		}


		for (_uint i = minIndex; i <= maxIndex; i++)
		{
			Add_Reset(resetIndex , i);
		}
	}

	void			Clear_Event()
	{
		Free();
	}


private:

	void	Free();

	list<EVENT*> m_listCurActiveEvents;
	vector<vector<int>> m_vecResetList;

	map<_uint, EVENT*> m_mapEvents;

	_bool m_bEventSwitch[1000];

};

END
