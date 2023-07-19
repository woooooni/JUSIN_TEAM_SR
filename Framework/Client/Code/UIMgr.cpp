#include "Export_Function.h"
#include "UIMgr.h"


IMPLEMENT_SINGLETON(CUIMgr)
CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
    Free();
}

HRESULT CUIMgr::Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    m_pDialog = CUI_Dialog::Create(_pGraphicDev);
    m_pHpBar = CUI_HPBar::Create(_pGraphicDev);
    m_pShop = CUI_Shop::Create(_pGraphicDev);
    m_pShortCutKey = CUI_ShortCutKey::Create(_pGraphicDev);

    NULL_CHECK_RETURN(m_pDialog, E_FAIL);
    NULL_CHECK_RETURN(m_pHpBar, E_FAIL);
    NULL_CHECK_RETURN(m_pShop, E_FAIL);
    NULL_CHECK_RETURN(m_pShortCutKey, E_FAIL);

    return S_OK;
}

void CUIMgr::Update_UIMgr(const _float& fTimeDelta)
{
    // TODO 
}

void CUIMgr::Late_Update_UIMgr()
{
}

void CUIMgr::Render_UIMgr()
{
}

void CUIMgr::Free()
{
    // TODO
}
