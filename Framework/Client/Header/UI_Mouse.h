#pragma once
#include "CUI.h"

class CUI_Mouse : public CUI
{
	CLONE(CUI_Mouse)

private:
	CUI_Mouse(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_Mouse(const CUI_Mouse& rhs);
	virtual ~CUI_Mouse();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();

public:
	static  CUI_Mouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

