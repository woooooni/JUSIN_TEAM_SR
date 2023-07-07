#pragma once
#include "CUI.h"

class CUIPages;
class CUI_BookBackground;

class CUI_AdventureBook :
    public CUI
{

private:
    explicit CUI_AdventureBook(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CUI_AdventureBook(const CUI_AdventureBook& rhs);
    virtual ~CUI_AdventureBook();

public:
    CUIPages* m_pUIPages;
    CUI_BookBackground* m_pUIBackGround;

public:
    static  CUIPages*   Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void		Free() override;

};

