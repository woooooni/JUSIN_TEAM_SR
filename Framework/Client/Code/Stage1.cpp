#include "Stage1.h"

CStage1::CStage1(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev, SCENE_TYPE::STAGE1)
{
}

CStage1::~CStage1()
{
}

HRESULT CStage1::Ready_Scene()
{
    __super::Ready_AllLayer();

    return S_OK;
}

_int CStage1::Update_Scene(const _float& fTimeDelta)
{
    return _int();
}

void CStage1::LateUpdate_Scene()
{
}

void CStage1::Render_Scene()
{
}

CStage1* CStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage1* ret = new CStage1(pGraphicDev);

    return nullptr;
}

void CStage1::Free()
{
}

HRESULT CStage1::Ready_Layer_Player()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_Camera()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_Terrrain()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_Environment()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_Monster()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_InterationObj()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_Effect()
{
    return E_NOTIMPL;
}

HRESULT CStage1::Ready_Layer_UI()
{
    return E_NOTIMPL;
}
