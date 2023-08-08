#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_SlotTwo : public CUI
{
	CLONE(CUI_SlotTwo)

private:
	explicit CUI_SlotTwo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SlotTwo(const CUI_SlotTwo& rhs);
	virtual ~CUI_SlotTwo();

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
	static  CUI_SlotTwo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

