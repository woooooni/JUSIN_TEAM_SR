#include "../Include/stdafx.h"
#include "Player_State_Push.h"

CPlayer_State_Push::CPlayer_State_Push(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Push::~CPlayer_State_Push()
{
}

HRESULT CPlayer_State_Push::Ready_State(void)
{
	return E_NOTIMPL;
}

_int CPlayer_State_Push::Update_State(const _float& fTimeDelta)
{
	return _int();
}

void CPlayer_State_Push::LateUpdate_State(void)
{
}

void CPlayer_State_Push::Render_State(void)
{
}
