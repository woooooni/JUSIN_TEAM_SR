#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include	"Item.h"

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

	HRESULT	Add_Item(CGameObject* pItem);

	_bool	Is_In_Inven(CGameObject* pItem);
private:
	vector<CItem*> m_vecInventory[(_uint)INVENTORY_TYPE::INVENTORY_END];

public:
	virtual void Free() override;
};

