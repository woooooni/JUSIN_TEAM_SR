#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CGameObject;
END
class CInventoryMgr : public CBase
{
	DECLARE_SINGLETON(CInventoryMgr)

public:
	enum class INVENTORY_TYPE { EQUIPMENT, CONSUMPSION, ETC, INVENTORY_END };

private:
	explicit CInventoryMgr();
	virtual ~CInventoryMgr();

public:
	HRESULT Ready_InvenMgr(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	vector<CGameObject*> m_vecInventory[(_uint)INVENTORY_TYPE::INVENTORY_END];

public:
	virtual void Free() override;
};

