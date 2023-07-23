#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "GameObject.h"



BEGIN(Engine)
typedef struct tagEvt
{
	EVENT_TYPE eEvent;
	DWORD_PTR lParam;
	DWORD_PTR wParam;
} EVENT_DESC;

class ENGINE_DLL CEventMgr : public CBase
{
	DECLARE_SINGLETON(CEventMgr)

private:
	explicit CEventMgr();
	virtual ~CEventMgr();

public:
	HRESULT Ready_EventMgr();
	void Update_EventMgr(const _float& fTimeDelta);
	
public:
	void DeleteObjEvt(CGameObject* pObj)
	{
		EVENT_DESC evt;
		pObj->Set_Active(false);
		evt.lParam = (DWORD_PTR)pObj;
		evt.eEvent = EVENT_TYPE::DELETE_OBJECT;

		AddEvent(evt);
	}

public:
	vector<CGameObject*>& Get_DelteObj_Vec(OBJ_TYPE _eObjType) { return m_vecDeleteObj[(_uint)_eObjType]; }


private:
	void AddEvent(const EVENT_DESC& _tDesc);
	void Excute(const EVENT_DESC& _tDesc);

private:
	vector<CGameObject*> m_vecDeleteObj[(_uint)OBJ_TYPE::OBJ_END];
	vector<EVENT_DESC> m_vecEvent;
public:
	virtual void Free() override;
};
END
