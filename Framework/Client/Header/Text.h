#pragma once
#include "CUI.h"

class CTextBox : public CUI
{
	CLONE(CTextBox)

protected:
	explicit CTextBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTextBox(const CTextBox& rhs);
	virtual ~CTextBox();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CTextBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

