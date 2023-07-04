#include "../Include/stdafx.h"
#include "InteractiveObj.h"
#include "Engine_Define.h"



CInteractiveObj::CInteractiveObj(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_ENVIRONMENT)
{
}

CInteractiveObj::CInteractiveObj(const CInteractiveObj& rhs) : CGameObject(rhs)
{
}

CInteractiveObj::~CInteractiveObj()
{
}
