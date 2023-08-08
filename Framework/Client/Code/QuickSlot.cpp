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
	m_vecSlots.resize(4);
	m_vecSetCode.resize(4);
	m_vecCount.resize(4);
	m_vecInvenIndex.resize(4);
	m_vecInSlot.resize(4);

	for (auto& iter : m_vecInSlot)
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

	for (auto& iter : m_vecInvenIndex)
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

	if (m_bCanUse) // Inventory창이 꺼져 있음.
	{
		vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();

			for (_int i = 0; i < vecItem.size(); i++) // 인벤토리 내에 있는 아이템 중 슬롯 아이템과 일치하는 것이 있으면
			{
				ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();
				ITEM_CODE iInvenCode = 
				CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

				if (iInvenCode == iSlotCode)
				{
					m_vecInSlot[SLOT_ONE] = iInvenCode; // 아이템 코드를 저장한다.
					m_vecCount[SLOT_ONE] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
				}

				else
					m_vecInSlot[SLOT_ONE] = ITEM_CODE::ITEM_END; // 그렇지 않으면 아이템이 없다는 뜻이므로 ITEM_END
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();

			for (_int i = 0; i < vecItem.size(); i++)
			{
				ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode();
				ITEM_CODE iInvenCode =
					CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

				if (iInvenCode == iSlotCode)
				{
					m_vecInSlot[SLOT_TWO] = iInvenCode;
					m_vecCount[SLOT_TWO] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
				}

				else
					m_vecInSlot[SLOT_TWO] = ITEM_CODE::ITEM_END;
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

			for (_int i = 0; i < vecItem.size(); i++)
			{
				ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode();
				ITEM_CODE iInvenCode =
					CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

				if (iInvenCode == iSlotCode)
				{
					m_vecInSlot[SLOT_THREE] = iInvenCode;
					m_vecCount[SLOT_THREE] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
				}

				else
					m_vecInSlot[SLOT_THREE] = ITEM_CODE::ITEM_END;
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();

			for (_int i = 0; i < vecItem.size(); i++)
			{
				ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();
				ITEM_CODE iInvenCode =
					CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

				if (iInvenCode == iSlotCode)
				{
					m_vecInSlot[SLOT_FOUR] = iInvenCode; 
					m_vecCount[SLOT_FOUR] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
				}

				else
					m_vecInSlot[SLOT_FOUR] = ITEM_CODE::ITEM_END;
			}
		}

		if (KEY_TAP(KEY::NUM_1))
		{
			if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
			{
				_uint iCode = _uint(dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode());

				if (iCode <= 5) // 소비아이템인 경우
				{
					if (m_vecCount[SLOT_ONE] > 0)
						m_vecCount[SLOT_ONE] -= 1;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Use_Item();
				}

				if (iCode >= 10 && iCode <= 15) // 장비 아이템인 경우
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

	if (!m_bCanUse) // Inventory가 열려있을때
	{
		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled()) // 1번 슬롯이 차있으면 아이템이 등록되었다는 이야기
		{
			vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();
			ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();

			if (_uint(iSlotCode) <= 5
				&& ITEM_CODE::ITEM_END != dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode()) // ITEM_CODE가 ITEM_END가 아니라면
			{
				for (_int i = 0; i < vecItem.size(); i++)
				{
					ITEM_CODE iInvenCode = // 인벤토리 내의 모든 아이템과 code를 비교해서
						CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

					if (iInvenCode == iSlotCode) // 두 코드가 같다면 InvenCount도 일치할 것.
					{
						m_vecInvenIndex[SLOT_ONE] = i;
						m_vecInSlot[SLOT_ONE] = iInvenCode;
						m_vecCount[SLOT_ONE] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
					}

					else
						m_vecInSlot[SLOT_ONE] = ITEM_CODE::ITEM_END;
				}

				if (_uint(iSlotCode) <= 5
					&& iSlotCode != CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[m_vecInvenIndex[SLOT_ONE]]->Get_ItemCode())
				{
					m_vecInSlot[SLOT_ONE] = ITEM_CODE::ITEM_END;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}
		}


		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_Filled())
		{
			vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();
			ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode();

			if (_uint(iSlotCode) <= 5
				&& ITEM_CODE::ITEM_END != dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_ItemCode()) // ITEM_CODE가 ITEM_END가 아니라면
			{
				for (_int i = 0; i < vecItem.size(); i++)
				{
					ITEM_CODE iInvenCode = // 인벤토리 내의 모든 아이템과 code를 비교해서
						CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

					if (iInvenCode == iSlotCode) // 두 코드가 같다면 InvenCount도 일치할 것.
					{
						m_vecInvenIndex[SLOT_TWO] = i;
						m_vecInSlot[SLOT_TWO] = iInvenCode;
						m_vecCount[SLOT_TWO] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
					}

					else
						m_vecInSlot[SLOT_TWO] = ITEM_CODE::ITEM_END;
				}

				if (_uint(iSlotCode) <= 5
					&& iSlotCode != CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[m_vecInvenIndex[SLOT_TWO]]->Get_ItemCode())
				{
					m_vecInSlot[SLOT_TWO] = ITEM_CODE::ITEM_END;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_Filled())
		{
			vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();
			ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode();

			if (_uint(iSlotCode) <= 5
				&& ITEM_CODE::ITEM_END != dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_ItemCode()) // ITEM_CODE가 ITEM_END가 아니라면
			{
				for (_int i = 0; i < vecItem.size(); i++)
				{
					ITEM_CODE iInvenCode = // 인벤토리 내의 모든 아이템과 code를 비교해서
						CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

					if (iInvenCode == iSlotCode) // 두 코드가 같다면 InvenCount도 일치할 것.
					{
						m_vecInvenIndex[SLOT_THREE] = i;
						m_vecInSlot[SLOT_THREE] = iInvenCode;
						m_vecCount[SLOT_THREE] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
					}

					else
						m_vecInSlot[SLOT_THREE] = ITEM_CODE::ITEM_END;
				}

				if (_uint(iSlotCode) <= 5
					&& iSlotCode != CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[m_vecInvenIndex[SLOT_THREE]]->Get_ItemCode())
				{
					m_vecInSlot[SLOT_THREE] = ITEM_CODE::ITEM_END;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}
		}

		if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_Filled())
		{
			vector<CItem*> vecItem = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();
			ITEM_CODE iSlotCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode();

			if (_uint(iSlotCode) <= 5
				&& ITEM_CODE::ITEM_END != dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_ItemCode()) // ITEM_CODE가 ITEM_END가 아니라면
			{
				for (_int i = 0; i < vecItem.size(); i++)
				{
					ITEM_CODE iInvenCode = // 인벤토리 내의 모든 아이템과 code를 비교해서
						CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_ItemCode();

					if (iInvenCode == iSlotCode) // 두 코드가 같다면 InvenCount도 일치할 것.
					{
						m_vecInvenIndex[SLOT_FOUR] = i;
						m_vecInSlot[SLOT_FOUR] = iInvenCode;
						m_vecCount[SLOT_FOUR] = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[i]->Get_InvenCount();
					}

					else
						m_vecInSlot[SLOT_FOUR] = ITEM_CODE::ITEM_END;
				}

				if (_uint(iSlotCode) <= 5 &&
					iSlotCode != CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[m_vecInvenIndex[SLOT_FOUR]]->Get_ItemCode())
				{
					m_vecInSlot[SLOT_FOUR] = ITEM_CODE::ITEM_END;
					dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Set_ItemCode(ITEM_CODE::ITEM_END);
				}
			}
		}
	}

	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	_uint iCount = 0;
	wstring strCount = L"";

	if (dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_Filled())
	{
		ITEM_CODE eCode = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END)
		{
			m_vecInSlot[SLOT_ONE] = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_ItemCode();
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_ONE])->Get_InvenIndex();
			
			if (m_vecCount[SLOT_ONE] >= 0)
			{
				RECT rc = { 85, 95, 135, 145 };
				strCount = to_wstring(m_vecCount[SLOT_ONE]);

				if (strCount != L"1" && strCount != L"0")
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

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_TWO])->Get_InvenIndex();

			if (m_vecCount[SLOT_TWO] >= 0)
			{
				RECT rc = { 162, 95, 212, 145 };
				strCount = to_wstring(m_vecCount[SLOT_TWO]);

				if (strCount != L"1" && strCount != L"0")
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

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_THREE])->Get_InvenIndex();

			if (m_vecCount[SLOT_THREE] >= 0)
			{

				RECT rc = { 239, 95, 289, 145 };
				strCount = to_wstring(m_vecCount[SLOT_THREE]);

				if (strCount != L"1" && strCount != L"0")
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

		if (_uint(eCode) <= 5 && eCode != ITEM_CODE::ITEM_END)
		{
			_uint iIndex = dynamic_cast<CUI_SlotItems*>(m_vecSlots[SLOT_FOUR])->Get_InvenIndex();

			if (m_vecCount[SLOT_FOUR] >= 0)
			{

				RECT rc = { 316, 95, 366, 145 };
				strCount = to_wstring(m_vecCount[SLOT_FOUR]);

				if (strCount != L"1" && strCount != L"0")
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
	//m_vecInSlot[_eSlotNum] = _eCodeType;
	dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Set_ItemCode(_eCodeType);
	dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Set_InvenIndex(_iInvenIndex);

	Stop_Sound(CHANNELID::SOUND_UI);
	Play_Sound(L"SFX_448_QuickSlotAdd.wav", CHANNELID::SOUND_UI, 0.9f);
}

_bool CQuickSlot::Get_Filled(SLOTNUM _eSlotNum)
{
	return dynamic_cast<CUI_SlotItems*> (m_vecSlots[(_uint)_eSlotNum])->Get_Filled();
}

void CQuickSlot::Set_Filled(SLOTNUM _eSlotNum, _bool _bFilled)
{
	dynamic_cast<CUI_SlotItems*>(m_vecSlots[(_uint)_eSlotNum])->Set_Filled(_bFilled);
}

void CQuickSlot::Set_SetCode(SLOTNUM _eSlotNum, _bool _bSetCode)
{
	m_vecSetCode[_eSlotNum] = _bSetCode;
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
