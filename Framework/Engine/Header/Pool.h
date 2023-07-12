#pragma once
/*

	* ���ǻ���

	* �Լ� ȣ��� ������ Ÿ���� �ƴ� '�� ������' Ÿ���� �Ѱ��־�� �Ѵ�.

	* ex)
			C
			<CDamageUI>::Init(OBJ_TYPE::UI, 50);

			CDamageUI* pUI = static_cast<CDamageUI*>(CPool<CDamageUI>::Get_Obj());

*/

#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

template <typename T>

class ENGINE_DLL CPool : public CBase
{
public:
	CPool() {}
	~CPool() { Free();  __super::Free(); }

public:
	static HRESULT Ready_Pool(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& _iSize)
	{
		for (_uint i = 0; i < _iSize; ++i)
		{
			CGameObject* pObj = T::Create(pGraphicDev);
			NULL_CHECK_RETURN(pObj, E_FAIL);
			pObj->Set_Active(false);
			g_ObjQueue.push(pObj);
		}
		return S_OK;
	}

	static CGameObject* Get_Obj()
	{
		if (g_ObjQueue.empty()) return nullptr;

		CGameObject* pObj = g_ObjQueue.front();
		NULL_CHECK_RETURN(pObj, E_FAIL);

		g_ObjQueue.pop();

		pObj->Set_Active(true);

		return pObj;
	}

	static _bool Return_Obj(CGameObject* _pObj)
	{
		NULL_CHECK_RETURN(pObj, E_FAIL);
		_pObj->Set_Active(false);
		g_ObjQueue.push(_pObj);

		return TRUE;
	}

	void Free()
	{
		if (g_ObjQueue.empty()) return;

		_int iSize = g_ObjQueue.size();

		for (_int i = 0; i < iSize; ++i)
		{
			CGameObject* pObj = g_ObjQueue.front();
			Safe_Release(pObj);
			g_ObjQueue.pop();
		}
	}

private:
	static queue<CGameObject*> g_objQueue;     // ��Ȱ��ȭ�� ������Ʈ�鸸 �����ִ�.
};

template <typename T>
queue<CGameObject*> CPool<T>::g_objQueue;

END