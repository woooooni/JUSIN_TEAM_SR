#include "../Include/stdafx.h"
#include "PushableObj.h"
#include "Engine_Define.h"



CPushableObj::CPushableObj(LPDIRECT3DDEVICE9 p_Dev, OBJ_ID _eID) : CFieldObject(p_Dev, _eID)
{
	m_tInfo.m_bIsPushable = true;
}

CPushableObj::CPushableObj(const CPushableObj& rhs) : CFieldObject(rhs)
{
}

CPushableObj::~CPushableObj()
{
}
