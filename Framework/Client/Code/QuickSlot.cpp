#include "QuickSlot.h"
#include "Export_Function.h"
#include "UI_SlotItems.h"
#include "InventoryMgr.h"
#include "Item.h"
#include "InventoryUI.h"
#include "Player.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_bCanUse(true)
{
	m_vecSlotItems.resize(4);
	m_vecSlots.resize(4);
	m_vecSetCode.resize(4);
	m_vecCount.resize(4);

	for (auto& iter : m_vecSlotItems)
	{
		iter = ITEM_CODE::ITEM_END;
	}

	for (auto& iter : m_vecSetCode)
	{
		iter = false;
	}

	for (auto& iter : m_vecSlots)
	{
		iter = nullptr;
	}

	for (auto& iter : m_vecCount)
	{
		iter = 0;
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
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player"));
	
	if (!pPlayer)
		return E_FAIL;

	if (m_bCanUse)
	{
		if (KEY_TAP(KEY::NUM_1))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
			{
				_uint iCode = _uint(dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode());

				if (iCode <= 5)
				{
					if (m_vecCount[SLOT_ONE] > 0)
						m_vecCount[SLOT_ONE] -= 1;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Use_Item();
				}

				if (iCode >= 10 && iCode <= 15)
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Wear_Item();
			}
		}

		if (KEY_TAP(KEY::NUM_2))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
			{
				_uint iCode = _uint(dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode());

				if (iCode <= 5)
				{
					if (m_vecCount[SLOT_TWO] > 0)
						m_vecCount[SLOT_TWO] -= 1;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Use_Item();
				}

				if (iCode >= 10 && iCode <= 15)
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Wear_Item();
			}
		}

		if (KEY_TAP(KEY::NUM_3))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
			{
				_uint iCode = _uint(dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode());

				if (iCode <= 5)
				{
					if (m_vecCount[SLOT_THREE] > 0)
						m_vecCount[SLOT_THREE] -= 1;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Use_Item();
				}

				if (iCode >= 10 && iCode <= 15)
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Wear_Item();
			}
		}

		if (KEY_TAP(KEY::NUM_4))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
			{
				_uint iCode = _uint(dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode());

				if (iCode <= 5)
				{
					if (m_vecCount[SLOT_FOUR] > 0)
						m_vecCount[SLOT_FOUR] -= 1;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Use_Item();
				}

				if (iCode >= 10 && iCode <= 15)
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Wear_Item();
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

	if (!m_bCanUse) // 인벤토리가 열려있지 않을 때
	{
		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
		{
			if (m_vecSetCode[SLOT_ONE] == true)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();

				if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode()
					!= CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}

				if (m_vecCount[SLOT_ONE] <= 0)
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}

			else if (m_vecSetCode[SLOT_ONE] == false)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();
				vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
				
				if (vecItem.size() > iIndex)
				{
					if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode()
						== CInventoryMgr::GetInstance()->
						Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
					{
						m_vecCount[SLOT_ONE] = CInventoryMgr::GetInstance()->
							Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_InvenCount();

						if (m_vecCount[SLOT_ONE] <= 1)
							m_vecSetCode[SLOT_ONE] = true;

						m_vecSlotItems[SLOT_ONE] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();
					}
				}
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
		{
			if (m_vecSetCode[SLOT_TWO] == true)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();

				if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode()
					!= CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}

				if (m_vecCount[SLOT_TWO] <= 0)
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}

			else if (m_vecSetCode[SLOT_TWO] == false)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();
				vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
				
				if (vecItem.size() > iIndex)
				{
					if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode()
						== CInventoryMgr::GetInstance()->
						Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
					{
						m_vecCount[SLOT_TWO] = CInventoryMgr::GetInstance()->
							Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_InvenCount();

						if (m_vecCount[SLOT_TWO] <= 1)
							m_vecSetCode[SLOT_TWO] = true;

						m_vecSlotItems[SLOT_TWO] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode();
					}
				}
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled() && m_vecSetCode[SLOT_THREE] == false)
		{
			if (m_vecSetCode[SLOT_THREE] == true)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

				if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode()
					!= CInventoryMgr::GetInstance()->
					Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}

				if (m_vecCount[SLOT_THREE] <= 0)
				{
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}

			else if (m_vecSetCode[SLOT_THREE] == false)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

				vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);

				if (vecItem.size() > iIndex)
				{
					if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode()
						== CInventoryMgr::GetInstance()->
						Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
					{
						m_vecCount[SLOT_THREE] = CInventoryMgr::GetInstance()->
							Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_InvenCount();

						if (m_vecCount[SLOT_THREE] <= 1)
							m_vecSetCode[SLOT_THREE] = true;

						m_vecSlotItems[SLOT_THREE] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode();
					}
				}
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
		{
			if (m_vecSetCode[SLOT_FOUR] == true)
			{
				if (m_vecCount[SLOT_FOUR] <= 0)
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Set_ItemCode(ITEM_CODE::ITEM_END);
			}

			else if (m_vecSetCode[SLOT_FOUR] == false)
			{
				_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();
				vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);

				if (vecItem.size() > iIndex)
				{
					if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode()
						== CInventoryMgr::GetInstance()->
						Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_ItemCode())
					{
						m_vecCount[SLOT_FOUR] = CInventoryMgr::GetInstance()->
							Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_InvenCount();

						if (m_vecCount[SLOT_FOUR] <= 1)
							m_vecSetCode[SLOT_FOUR] = true;

						m_vecSlotItems[SLOT_FOUR] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();
					}
				}
			}

			// 기존 form
//			_uint iIndex = m_vecCount[SLOT_FOUR] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();
//
//			m_vecCount[SLOT_FOUR] = CInventoryMgr::GetInstance()->
//				Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[iIndex]->Get_InvenCount();
//
//			if (m_vecCount[SLOT_FOUR] <= 1)
//				m_vecSetCode[SLOT_FOUR] = true;
//
//			m_vecSlotItems[SLOT_FOUR] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();
		}
	}

	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	// 인덱스 관련 수정 진행중
	// Get_Filed가 하나라도 false가 되었을때
	_uint iCount = 0;
	wstring strCount = L"";

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END && m_vecSlotItems[SLOT_ONE] == eCode)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();
			
			if (m_vecCount[SLOT_ONE] >= 0)
			{
				RECT rc = { 85, 95, 135, 145 };
				strCount = to_wstring(m_vecCount[SLOT_ONE]);

				if (strCount != L"1")
				{
					Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
						->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
							DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
							D3DCOLOR_ARGB(150, 0, 0, 0));
				}
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode();

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END && m_vecSlotItems[SLOT_TWO] == eCode)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();

			if (m_vecCount[SLOT_TWO] >= 0)
			{
				RECT rc = { 162, 95, 212, 145 };
				strCount = to_wstring(m_vecCount[SLOT_TWO]);

				if (strCount != L"1")
				{
					Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
						->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
							DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
							D3DCOLOR_ARGB(150, 0, 0, 0));
				}
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode();

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END && m_vecSlotItems[SLOT_THREE] == eCode)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

			if (m_vecCount[SLOT_THREE] >= 0)
			{

				RECT rc = { 239, 95, 289, 145 };
				strCount = to_wstring(m_vecCount[SLOT_THREE]);

				if (strCount != L"1")
				{
					Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
						->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
							DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
							D3DCOLOR_ARGB(150, 0, 0, 0));
				}
			}
		}
	}

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END && m_vecSlotItems[SLOT_FOUR] == eCode)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();

			if (m_vecCount[SLOT_FOUR] >= 0)
			{

				RECT rc = { 316, 95, 366, 145 };
				strCount = to_wstring(m_vecCount[SLOT_FOUR]);

				if (strCount != L"1")
				{
					Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
						->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
							DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
							D3DCOLOR_ARGB(150, 0, 0, 0));
				}
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
