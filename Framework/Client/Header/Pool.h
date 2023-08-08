#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

template <typename T>
class CPool : public CBase
{
public:
	CPool() {}
	~CPool() 
	{ 
		Free();  
		__super::Free(); 
	}

public:
	static HRESULT Ready_Pool(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& _iSize)
	{
		for (_uint i = 0; i < _iSize; ++i)
		{
			T* pObj = T::Create(pGraphicDev);
			NULL_CHECK_RETURN(pObj, E_FAIL);
			pObj->Set_Active(false);
			g_objQueue.push(pObj);
		}
		return S_OK;
	}

	static T* Get_Obj()
	{
		if (g_objQueue.empty()) return nullptr;

		T* pObj = g_objQueue.front();
		NULL_CHECK_RETURN(pObj, nullptr);

		g_objQueue.pop();

		pObj->Set_Active(true);

		return pObj;
	}

	static _bool Return_Obj(T* _pObj)
	{
		NULL_CHECK_RETURN(_pObj, FALSE);
		_pObj->Set_Active(false);
		g_objQueue.push(_pObj);

		return TRUE;
	}

	void Free()
	{
		if (g_objQueue.empty()) 
			return;

		_int iSize = g_objQueue.size();

		for (_int i = 0; i < iSize; ++i)
		{
			T* pObj = g_objQueue.front();
			Safe_Release(pObj);
			g_objQueue.pop();
		}
	}

	static queue<T*>& Get_Que() { return g_objQueue; }

private:
	static queue<T*> g_objQueue;     // 비활성화된 오브젝트들만 갖고있다.
};

template <typename T>
queue<T*> CPool<T>::g_objQueue;

END