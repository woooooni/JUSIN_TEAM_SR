#include "../Include/stdafx.h"
#include "FieldObject.h"

CFieldObject::CFieldObject(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}

CFieldObject::CFieldObject(const CFieldObject& rhs) : CGameObject(rhs), m_tInfo(rhs.m_tInfo)
{
}

CFieldObject::~CFieldObject()
{
}
