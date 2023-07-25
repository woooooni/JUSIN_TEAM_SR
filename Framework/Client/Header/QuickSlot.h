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
	HRESULT			Add_Slot(void);
	void			Set_Item(SLOTNUM _eSlotNum, ITEM_CODE _eCodeType);
	_bool			Get_Filled(SLOTNUM _eSlotNum);

public:

private:
	vector<CUI*>	m_vecSlots;
	vector<ITEM_CODE>	m_vecSlotItems;

private: // 슬롯이 차있는지 확인하는 목적의 bool변수.
	_bool			m_bFilledSlot1 = false;
	_bool			m_bFilledSlot2 = false;
	_bool			m_bFilledSlot3 = false;
	_bool			m_bFilledSlot4 = false;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Set_Use(const _bool& pBool)
	{
		m_bCanUse = pBool;
	}
private:
	virtual void		Free() override;
	_bool		m_bCanUse;
};

