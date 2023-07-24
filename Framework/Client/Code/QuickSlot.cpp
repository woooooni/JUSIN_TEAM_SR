#include "QuickSlot.h"
#include "Export_Function.h"
#include "UI_SlotItems.h"
#include "InventoryMgr.h"
#include "Item.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CQuickSlot::CQuickSlot(const CQuickSlot& rhs)
	: CUI(rhs)
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
	m_vecSlots[SLOT_ONE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_TWO]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_THREE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_FOUR]->Update_Object(fTimeDelta);

	if (m_pSlotItem1 != nullptr)
		m_pSlotItem1->Update_Object(fTimeDelta);

	if (m_pSlotItem2 != nullptr)
		m_pSlotItem2->Update_Object(fTimeDelta);

	if (m_pSlotItem3 != nullptr)
		m_pSlotItem3->Update_Object(fTimeDelta);

	if (m_pSlotItem4 != nullptr)
		m_pSlotItem4->Update_Object(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CQuickSlot::LateUpdate_Object(void)
{
	m_vecSlots[SLOT_ONE]->LateUpdate_Object();
	m_vecSlots[SLOT_TWO]->LateUpdate_Object();
	m_vecSlots[SLOT_THREE]->LateUpdate_Object();
	m_vecSlots[SLOT_FOUR]->LateUpdate_Object();

	if (m_pSlotItem1 != nullptr)
		m_pSlotItem1->LateUpdate_Object();

	if (m_pSlotItem2 != nullptr)
		m_pSlotItem2->LateUpdate_Object();

	if (m_pSlotItem3 != nullptr)
		m_pSlotItem3->LateUpdate_Object();

	if (m_pSlotItem4 != nullptr)
		m_pSlotItem4->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CQuickSlot::Add_Slot(void)
{
	m_vecSlots.reserve(SLOTNUM::SLOT_END);

	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI);

	m_pSlotOne = CUI_SlotOne::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSlotOne, E_FAIL);
	m_vecSlots.push_back(m_pSlotOne);

	m_pSlotTwo = CUI_SlotTwo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSlotTwo, E_FAIL);
	m_vecSlots.push_back(m_pSlotTwo);

	m_pSlotThree = CUI_SlotThree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSlotThree, E_FAIL);
	m_vecSlots.push_back(m_pSlotThree);

	m_pSlotFour = CUI_SlotFour::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSlotFour, E_FAIL);
	m_vecSlots.push_back(m_pSlotFour);

	return S_OK;
}

void CQuickSlot::Set_Item(SLOTNUM _eSlotNum, ITEM_CODE _eCodeType)
{
	float fTimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

	switch (_eSlotNum)
	{
	case SLOTNUM::SLOT_ONE:
		if (!m_bFilledSlot1)
		{
			m_bFilledSlot1 = true;

			m_pSlotItem1 = CUI_SlotItems::Create(m_pGraphicDev, SLOTITEM_NUM::FIRST);
			if (m_pSlotItem1 != nullptr)
				m_pSlotItem1->Get_TextureCom()->Set_Idx((_uint)_eCodeType);

			m_pSlotOne->Set_ItemCode(_eCodeType);
		}
		break;

	case SLOTNUM::SLOT_TWO:
		if (!m_bFilledSlot2)
		{
			m_bFilledSlot2 = true;

			m_pSlotItem2 = CUI_SlotItems::Create(m_pGraphicDev, SLOTITEM_NUM::SECOND);
			if (m_pSlotItem2 != nullptr)
				m_pSlotItem2->Get_TextureCom()->Set_Idx((_uint)_eCodeType);

			m_pSlotTwo->Set_ItemCode(_eCodeType);
		}
		break;

	case SLOTNUM::SLOT_THREE:
		if (!m_bFilledSlot3)
		{
			m_bFilledSlot3 = true;

			m_pSlotItem3 = CUI_SlotItems::Create(m_pGraphicDev, SLOTITEM_NUM::THIRD);
			if (m_pSlotItem3 != nullptr)
				m_pSlotItem3->Get_TextureCom()->Set_Idx((_uint)_eCodeType);

			m_pSlotThree->Set_ItemCode(_eCodeType);
		}
		break;

	case SLOTNUM::SLOT_FOUR:
		if (!m_bFilledSlot4)
		{
			m_bFilledSlot4 = true;

			m_pSlotItem4 = CUI_SlotItems::Create(m_pGraphicDev, SLOTITEM_NUM::FOURTH);
			if (m_pSlotItem4 != nullptr)
				m_pSlotItem4->Get_TextureCom()->Set_Idx((_uint)_eCodeType);

			m_pSlotFour->Set_ItemCode(_eCodeType);
		}
		break;

	case SLOTNUM::SLOT_END:
		break;

	default:
		break;
	}
}

_bool CQuickSlot::Get_Filled(SLOTNUM _eSlotNum)
{
	switch (_eSlotNum)
	{
	case SLOTNUM::SLOT_ONE:
		return m_bFilledSlot1;
		break;

	case SLOTNUM::SLOT_TWO:
		return m_bFilledSlot2;
		break;

	case SLOTNUM::SLOT_THREE:
		return m_bFilledSlot3;
		break;

	case SLOTNUM::SLOT_FOUR:
		return m_bFilledSlot4;
		break;

	default:
		break;
	}
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
