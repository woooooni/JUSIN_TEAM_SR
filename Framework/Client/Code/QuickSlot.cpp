#include "QuickSlot.h"
#include "Export_Function.h"
#include "UI_SlotItems.h"
#include "InventoryMgr.h"
#include "Item.h"
#include "InventoryUI.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_bCanUse(true)
{
	m_vecSlotItems.resize(4);
	m_vecSlots.resize(4);

	for (auto& iter : m_vecSlotItems)
	{
		iter = ITEM_CODE::ITEM_END;
	}

	for (auto& iter : m_vecSlots)
	{
		iter = nullptr;
	}

}

CQuickSlot::CQuickSlot(const CQuickSlot& rhs)
	: CUI(rhs)
	,m_bCanUse(rhs.m_bCanUse)
{
}

CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Slot(), E_FAIL);

	return S_OK;
}

_int CQuickSlot::Update_Object(const _float& fTimeDelta)
{
	if (m_bCanUse)
	{
		if (KEY_TAP(KEY::NUM_1))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
			{
				dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Use_Item();
			}
		}
		if (KEY_TAP(KEY::NUM_2))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
			{
				dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Use_Item();
			}
		}
		if (KEY_TAP(KEY::NUM_3))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
			{
				dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Use_Item();
			}
		}
		if (KEY_TAP(KEY::NUM_4))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
			{
				dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Use_Item();
			}
		}
	}

	m_vecSlots[SLOT_ONE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_TWO]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_THREE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_FOUR]->Update_Object(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CQuickSlot::LateUpdate_Object(void)
{
	m_vecSlots[SLOT_ONE]->LateUpdate_Object();
	m_vecSlots[SLOT_TWO]->LateUpdate_Object();
	m_vecSlots[SLOT_THREE]->LateUpdate_Object();
	m_vecSlots[SLOT_FOUR]->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	_uint iCount = 0;
	wstring strCount = L"";

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();

		if (eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();

			if (CInventoryMgr::GetInstance()->
				Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
				->Get_InvenCount() >= 0)
			{
				iCount = CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
					->Get_InvenCount();
			}

			//RECT rc = { 110, 115, 160, 165 }; X - 25, Y - 20
			RECT rc = { 85, 95, 135, 145 };
			strCount = to_wstring(iCount);

			if ((strCount != L"1") && (iCount > 0))
			{
				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
					->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
						DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
						D3DCOLOR_ARGB(150, 0, 0, 0));
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode();
	
		if (eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();
			
			if (CInventoryMgr::GetInstance()->
				Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
				->Get_InvenCount() > 0)
			{
				iCount = CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
					->Get_InvenCount();
			}

			RECT rc = { 162, 95, 212, 145 };
			strCount = to_wstring(iCount);

			if ((strCount != L"1") && (iCount > 0))
			{
				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
					->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
						DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
						D3DCOLOR_ARGB(150, 0, 0, 0));
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode();

		if (eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

			if (CInventoryMgr::GetInstance()->
				Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
				->Get_InvenCount() > 0)
			{
				iCount = CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
					->Get_InvenCount();
			}

			RECT rc = { 239, 95, 289, 145 };
			strCount = to_wstring(iCount);

			if ((strCount != L"1") && (iCount > 0))
			{
				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
					->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
						DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
						D3DCOLOR_ARGB(150, 0, 0, 0));
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();

		if (eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();

			if (CInventoryMgr::GetInstance()->
				Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
				->Get_InvenCount() > 0)
			{
				iCount = CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]
					->Get_InvenCount();
			}

			RECT rc = { 316, 95, 366, 145 }; // X + 77
			strCount = to_wstring(iCount);

			if ((strCount != L"1") && (iCount > 0))
			{
				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
					->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
						DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
						D3DCOLOR_ARGB(150, 0, 0, 0));
			}
		}
	}

	__super::Render_Object();
}

HRESULT CQuickSlot::Add_Slot(void)
{
	m_vecSlots.reserve(SLOTNUM::SLOT_END);

	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI);

	for (_uint i = 0; i < 4; ++i)
	{
		m_vecSlots[i] = CUI_SlotItems::Create(m_pGraphicDev, (SLOTITEM_NUM)i);
	}

	return S_OK;
}

void CQuickSlot::Set_Item(SLOTNUM _eSlotNum, _uint _iInvenIndex, ITEM_CODE _eCodeType)
{
	dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Set_ItemCode(_eCodeType);
	dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Set_InvenIndex(_iInvenIndex);
}

_bool CQuickSlot::Get_Filled(SLOTNUM _eSlotNum)
{
	return dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Get_Filled();
}

void CQuickSlot::Set_Filled(SLOTNUM _eSlotNum, _bool _bFilled)
{
	dynamic_cast<CUI_SlotItems*>(m_vecSlots[(_uint)_eSlotNum])->Set_Filled(_bFilled);
}

const ITEM_CODE& CQuickSlot::Get_ItemCode(SLOTNUM _eSlotNum)
{
	return dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Get_ItemCode();
}

CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuickSlot* pInstance = new CQuickSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("QuickSlot Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CQuickSlot::Free()
{
}
