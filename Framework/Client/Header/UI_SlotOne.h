#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_SlotOne : public CUI
{
	CLONE(CUI_SlotOne)

private:
	explicit CUI_SlotOne(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SlotOne(const CUI_SlotOne& rhs);
	virtual ~CUI_SlotOne();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_ItemCode(ITEM_CODE eItemcode)
	{
		m_eItemCode = eItemcode;
	}

private:
	ITEM_CODE	m_eItemCode;

public:
	static  CUI_SlotOne* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

