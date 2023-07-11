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

    return S_OK;
}

void CUIMgr::Update_UIMgr(const _float& fTimeDelta)
{
    // TODO 
}

void CUIMgr::Free()
{
    // TODO
}
