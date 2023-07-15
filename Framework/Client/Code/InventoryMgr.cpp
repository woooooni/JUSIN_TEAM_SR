#include "Export_Function.h"
#include "..\Header\InventoryMgr.h"
#include	"Pool.h"


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
	for (auto& iter : m_vecInventory)
	{
		iter.reserve(15);
	}

	return S_OK;
}

HRESULT CInventoryMgr::Add_Item(CGameObject* pItem)
{
	CItem* tem = dynamic_cast<CItem*>(pItem);
	NULL_CHECK_RETURN(tem, E_FAIL);

	if (tem->Get_ItemType() >= ITEM_TYPE::COIN)
		return E_FAIL;

	auto& yourVec = m_vecInventory[(_uint)tem->Get_ItemType()];

	if (yourVec.size() >= 15)
		return E_FAIL;

	auto iter = find_if(yourVec.begin(), yourVec.end(), [&](CItem* inventem)
		{
			return inventem->Get_ItemCode() == tem->Get_ItemCode();
		});

	if (iter == yourVec.end())
	{
		tem->Set_Active(false);
		yourVec.push_back(tem);
	}
	else
	{
		tem->Add_Pool();
		(*iter)->Set_InvenCount();

	}
	
	return S_OK;
}


_bool CInventoryMgr::Is_In_Inven(CGameObject* pItem)
{
	CItem* tem = dynamic_cast<CItem*>(pItem);
	NULL_CHECK_RETURN(tem, false);

	if (tem->Get_ItemType() >= ITEM_TYPE::COIN)
		return false;

	auto& yourVec = m_vecInventory[(_uint)tem->Get_ItemType()];

	auto iter = find(yourVec.begin(), yourVec.end(), tem);

	if (iter == yourVec.end())
		return true;

	return false;
}



void CInventoryMgr::Free()
{
	for (auto& iter : m_vecInventory)
	{
		iter.clear();
	}
}
