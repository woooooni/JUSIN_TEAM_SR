#pragma once
#include "CUI.h"
#include "UI_SlotOne.h"
#include "UI_SlotTwo.h"
#include "UI_SlotThree.h"
#include "UI_SlotFour.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

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

private:
	vector<CUI*>	m_vecSlots;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

