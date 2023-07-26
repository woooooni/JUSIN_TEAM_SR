#pragma once
#include "CUI.h"
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
	HRESULT		Add_Slot(void);
	void		Set_Item(SLOTNUM _eSlotNum, _uint _iInvenIndex, ITEM_CODE _eCodeType);
	_bool		Get_Filled(SLOTNUM _eSlotNum);
	void		Set_Filled(SLOTNUM _eSlotNum, _bool _bFilled);
	const ITEM_CODE& Get_ItemCode(SLOTNUM _eSlotNum);
	void		Set_Use(const _bool& pBool) { m_bCanUse = pBool; }

	const vector<CUI*>	Get_Slots() { return m_vecSlots; }

private:
	vector<CUI*>		m_vecSlots;
	vector<ITEM_CODE>	m_vecSlotItems;
	_bool				m_bCanUse;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

