#pragma once
#include "CUI.h"
class CUI_AdventureBook :
    public CUI
{

private:
    explicit CUI_AdventureBook(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CUI_AdventureBook(const CUI_AdventureBook& rhs);
    virtual ~CUI_AdventureBook();

public:


};

