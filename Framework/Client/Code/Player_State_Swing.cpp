#include "../Include/stdafx.h"
#include "Player_State_Swing.h"

CPlayer_State_Swing::CPlayer_State_Swing(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Swing::~CPlayer_State_Swing()
{
}

HRESULT CPlayer_State_Swing::Ready_State(void)
{
	return E_NOTIMPL;
}

_int CPlayer_State_Swing::Update_State(const _float& fTimeDelta)
{
	return _int();
}

void CPlayer_State_Swing::LateUpdate_State(void)
{
}

void CPlayer_State_Swing::Render_State(void)
{
}
