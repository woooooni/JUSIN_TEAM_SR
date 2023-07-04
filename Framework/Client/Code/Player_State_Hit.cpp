#include "../Include/stdafx.h"
#include "Player_State_Hit.h"

CPlayer_State_Hit::CPlayer_State_Hit(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Hit::~CPlayer_State_Hit()
{
}

HRESULT CPlayer_State_Hit::Ready_State(void)
{
	return E_NOTIMPL;
}

_int CPlayer_State_Hit::Update_State(const _float& fTimeDelta)
{
	return _int();
}

void CPlayer_State_Hit::LateUpdate_State(void)
{
}

void CPlayer_State_Hit::Render_State(void)
{
}
