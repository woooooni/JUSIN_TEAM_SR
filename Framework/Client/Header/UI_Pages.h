#pragma once
#include "CUI.h"

class CUI_Pages : public CUI
{
	CLONE(CUI_Pages)

private:
    explicit CUI_Pages(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUI_Pages(const CUI_Pages& rhs);
    virtual ~CUI_Pages();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();
public:
	static  CUI_Pages*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

