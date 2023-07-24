#pragma once
#include "CUI.h"
#include "UI_SlotOne.h"
#include "UI_SlotTwo.h"
#include "UI_SlotThree.h"
#include "UI_SlotFour.h"
#include "UI_SlotItems.h"

enum SLOTNUM
{
	SLOT_ONE, SLOT_TWO, SLOT_THREE, SLOT_FOUR,
	SLOT_END
};

class CQuickSlot : public CUI
{
	CLONE(CQuickSlot)
protected:
	explicit CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuickSlot(const CQuickSlot& rhs);
	virtual ~CQuickSlot();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT			Add_Slot(void);
	void			Set_Item(SLOTNUM _eSlotNum, ITEM_CODE _eCodeType);
	_bool			Get_Filled(SLOTNUM _eSlotNum);

public:
	CUI_SlotOne*	Get_SlotOne() { return m_pSlotOne; }
	CUI_SlotTwo*	Get_SlotTwo() { return m_pSlotTwo; }
	CUI_SlotThree*	Get_SlotThree() { return m_pSlotThree; }
	CUI_SlotFour*	Get_SlotFour() { return m_pSlotFour; }

private:
	vector<CUI*>	m_vecSlots;
	vector<CUI*>	m_vecSlotItems;

private: // 슬롯이 차있는지 확인하는 목적의 bool변수.
	_bool			m_bFilledSlot1 = false;
	_bool			m_bFilledSlot2 = false;
	_bool			m_bFilledSlot3 = false;
	_bool			m_bFilledSlot4 = false;

private:
	CUI_SlotOne*	m_pSlotOne = nullptr;
	CUI_SlotTwo*	m_pSlotTwo = nullptr;
	CUI_SlotThree*	m_pSlotThree = nullptr;
	CUI_SlotFour*	m_pSlotFour = nullptr;

private:
	CUI_SlotItems*	m_pSlotItem1 = nullptr;
	CUI_SlotItems*  m_pSlotItem2 = nullptr;
	CUI_SlotItems*  m_pSlotItem3 = nullptr;
	CUI_SlotItems*  m_pSlotItem4 = nullptr;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

