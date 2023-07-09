#pragma once
#include "CUI.h"

class CUI_Pages;
class CUI_BookBackground;

class CUI_AdventureBook :
    public CUI
{

private:
    explicit CUI_AdventureBook(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CUI_AdventureBook(const CUI_AdventureBook& rhs);
    virtual ~CUI_AdventureBook();

public:
    HRESULT		Ready_Object(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual _int Update_Object(const _float& fTimeDelta) override;
    virtual void LateUpdate_Object(void) override;
    virtual void Render_Object(void) override;

private:
    CUI_Pages* m_pUIPages;
    CUI_BookBackground* m_pUIBackGround;

public:
    static  CUI_AdventureBook*   Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void		Free() override;

};

