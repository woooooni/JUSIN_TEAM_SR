#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include	"Item.h"

class CPlayer;

BEGIN(Engine)
class CGameObject;
END
class CInventoryMgr : public CBase
{
	DECLARE_SINGLETON(CInventoryMgr)

public:
	enum class INVENTORY_TYPE 
	{ 
		EQUIPMENT, CONSUMPSION, ETC, INVENTORY_END 
	};

private:
	explicit CInventoryMgr();
	virtual ~CInventoryMgr();

public:
	HRESULT Ready_InvenMgr(LPDIRECT3DDEVICE9 _pGraphicDev);

	HRESULT	Add_Item(CGameObject* pItem);

	_bool	Is_In_Inven(CGameObject* pItem);

	HRESULT		Use_Item(_uint pInt);

	void		Set_Player(CPlayer* pPlayer);

	const	vector<CItem*>& Get_Inventory(INVENTORY_TYPE p_Type) { return m_vecInventory[(_uint)p_Type]; }

	void		Erase_Item(const INVENTORY_TYPE& p_Type, const _uint& pIndex)
	{
		m_vecInventory[(_uint)p_Type].erase(m_vecInventory[(_uint)p_Type].begin() + pIndex);
	}
private:
	vector<CItem*> m_vecInventory[(_uint)INVENTORY_TYPE::INVENTORY_END];
	CPlayer* m_pPlayer;
	_bool	m_bHasRooted[(_uint)ITEM_CODE::ITEM_END];

public:
	virtual void Free() override;
};

