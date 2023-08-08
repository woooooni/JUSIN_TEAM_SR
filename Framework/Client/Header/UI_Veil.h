#pragma once
#include "CUI.h"

class CUI_Veil :
    public CUI
{
private:
    explicit CUI_Veil(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CUI_Veil(const CUI_Veil& rhs);
    virtual ~CUI_Veil();

public:
    virtual HRESULT		Ready_Object(void) override;
    virtual _int		Update_Object(const _float& fTimeDelta) override;
    virtual void		LateUpdate_Object(void) override;
    virtual void		Render_Object(void) override;

public:
    void Set_Alpha(_float _fAlpha) { m_fAlpha = _fAlpha; }
private:
    HRESULT Ready_Component();

public:
    static  CUI_Veil* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _float m_fAlpha;

private:
    virtual void		Free() override;
};

