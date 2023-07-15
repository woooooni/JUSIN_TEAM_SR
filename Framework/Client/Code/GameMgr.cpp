#include "GameMgr.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CGameMgr)
CGameMgr::CGameMgr()
	: m_pPlayer(nullptr)
{
}

CGameMgr::~CGameMgr()
{
	Free();
}

HRESULT CGameMgr::Ready_GameMgr(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	// m_pPlayer = CPlayer::Create(_pGraphicDev);
	return S_OK;
}

void CGameMgr::Free()
{
	Safe_Release(m_pPlayer);
}
