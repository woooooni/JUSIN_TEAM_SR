#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Player.h"

class CGameMgr : public CBase
{
	DECLARE_SINGLETON(CGameMgr)

private:
	explicit CGameMgr();
	virtual ~CGameMgr();

public:
	HRESULT Ready_GameMgr(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	CPlayer* Get_Player() { return m_pPlayer; }


private:
	CPlayer* m_pPlayer;

public:
	virtual void Free() override;
};

