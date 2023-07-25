#pragma once
#include "CUI.h"

enum SLOTITEM_NUM
{
	FIRST, SECOND, THIRD, FOURTH,
	SLOTITEM_END
};

class CUI_SlotItems : public CUI
{
	CLONE(CUI_SlotItems)

private:
	explicit CUI_SlotItems(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SlotItems(const CUI_SlotItems& rhs);
	virtual ~CUI_SlotItems();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_SlotNum(SLOTITEM_NUM eNum);

private:
	HRESULT	Add_Component(void);

public:
	SLOTITEM_NUM m_eSlotNum;

private:
	_bool	m_bFilledSlot1 = false;
	_bool	m_bFilledSlot2 = false;
	_bool	m_bFilledSlot3 = false;
	_bool	m_bFilledSlot4 = false;

public:
	static  CUI_SlotItems* Create(LPDIRECT3DDEVICE9 pGraphicDev, SLOTITEM_NUM eNum);

private:
	virtual void		Free() override;
};

