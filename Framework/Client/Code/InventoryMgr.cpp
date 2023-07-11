#include "Export_Function.h"
#include "..\Header\InventoryMgr.h"


IMPLEMENT_SINGLETON(CInventoryMgr)
CInventoryMgr::CInventoryMgr()
{
}

CInventoryMgr::~CInventoryMgr()
{
	Free();
}

HRESULT CInventoryMgr::Ready_InvenMgr(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	return S_OK;
}

void CInventoryMgr::Free()
{
	for (_uint i = 0; i < (_uint)INVENTORY_TYPE::INVENTORY_END; ++i)
		for_each(m_vecInventory[i].begin(), m_vecInventory[i].end(), CDeleteObj());
}
