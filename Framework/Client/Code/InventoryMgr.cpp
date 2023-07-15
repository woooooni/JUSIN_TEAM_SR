#include "Export_Function.h"
#include "..\Header\InventoryMgr.h"
#include	"Pool.h"
#include	"Player.h"
#include	"UseItem.h"


IMPLEMENT_SINGLETON(CInventoryMgr)
CInventoryMgr::CInventoryMgr() : m_pPlayer(nullptr)
{
	ZeroMemory(m_bHasRooted, sizeof(_bool) * (_uint)(ITEM_CODE::ITEM_END));
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
		tem->Set_InvenCount();
	}
	else
	{
		if (tem->Get_ItemType() == ITEM_TYPE::EQUIPMENT)
		{
			//Erase 추천
		}
		else
		{
			tem->Add_Pool();
		}
		(*iter)->Set_InvenCount();

	}

	if (!m_bHasRooted[(_uint)tem->Get_ItemCode()])
	{
		m_bHasRooted[(_uint)tem->Get_ItemCode()] = true;

		//최초 획득 이벤트 코드
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

HRESULT CInventoryMgr::Use_Item(_uint pInt)
{
	auto& tmp = m_vecInventory[(_uint)INVENTORY_TYPE::CONSUMPSION];
	if (pInt < 0 || pInt >= tmp.size())
		return E_FAIL;

	CUseItem* src = dynamic_cast<CUseItem*>(tmp[pInt]);
	NULL_CHECK_RETURN(src, E_FAIL);

	HRESULT ret = src->Use_Item(m_pPlayer);

	if (src->Get_InvenCount() == 0)
		tmp.erase(tmp.begin() + pInt);

	return ret;
}

void CInventoryMgr::Set_Player(CPlayer* pPlayer)
{
	NULL_CHECK(pPlayer);

	m_pPlayer = pPlayer;
}



void CInventoryMgr::Free()
{
	for (auto& iter : m_vecInventory)
	{
		iter.clear();
	}
}
