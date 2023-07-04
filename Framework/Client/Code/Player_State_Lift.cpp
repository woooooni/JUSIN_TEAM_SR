#include "../Include/stdafx.h"
#include "Player_State_Lift.h"

CPlayer_State_Lift::CPlayer_State_Lift(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Lift::~CPlayer_State_Lift()
{
}

HRESULT CPlayer_State_Lift::Ready_State(void)
{
	return E_NOTIMPL;
}

_int CPlayer_State_Lift::Update_State(const _float& fTimeDelta)
{
	return _int();
}

void CPlayer_State_Lift::LateUpdate_State(void)
{
}

void CPlayer_State_Lift::Render_State(void)
{
}
