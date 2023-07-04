#include "../Include/stdafx.h"
#include "PushableObj.h"
#include "Engine_Define.h"



CPushableObj::CPushableObj(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_ENVIRONMENT)
{
}

CPushableObj::CPushableObj(const CPushableObj& rhs) : CGameObject(rhs)
{
}

CPushableObj::~CPushableObj()
{
}
