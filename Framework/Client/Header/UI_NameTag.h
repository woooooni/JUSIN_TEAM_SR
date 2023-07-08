#pragma once
#include "CUI.h"

class CUI_NameTag : public CUI
{
private:
	CUI_NameTag(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_NameTag(const CUI_NameTag& rhs);
	virtual ~CUI_NameTag();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CUI_NameTag* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

