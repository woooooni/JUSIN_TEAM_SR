#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

typedef enum class SlotNum
{
	SLOT_ONE, SLOT_TWO, SLOT_THREE, SLOT_FOUR,
	SLOT_END

}SLOTNUM;

struct tagSlotInfo
{
	SLOTNUM		eType;
};

class CQuickSlot : public CUI
{
protected:
	explicit CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuickSlot(const CQuickSlot& rhs);
	virtual ~CQuickSlot();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			Set_Type(SLOTNUM eType);

private:
	tagSlotInfo		m_tInfo;

public:
	static  CQuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, SLOTNUM eType);

private:
	virtual void		Free() override;
};

