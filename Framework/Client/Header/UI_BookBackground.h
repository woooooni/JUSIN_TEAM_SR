#pragma once
#include "CUI.h"


class CUI_BookBackground :
    public CUI
{
private:
    explicit CUI_BookBackground(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CUI_BookBackground(const CUI_BookBackground& rhs);
    virtual ~CUI_BookBackground();

public:
    virtual HRESULT		Ready_Object(void) override;
    virtual _int		Update_Object(const _float& fTimeDelta) override;
    virtual void		LateUpdate_Object(void) override;
    virtual void		Render_Object(void) override;

private:
    HRESULT Ready_Component();

public:
    static  CUI_BookBackground* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void		Free() override;
};

