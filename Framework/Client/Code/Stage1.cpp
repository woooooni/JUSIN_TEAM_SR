#include "Export_Function.h"
#include "../Include/stdafx.h"
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
    m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f));
    m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

    return S_OK;
}

_int CStage1::Update_Scene(const _float& fTimeDelta)
{
    return __super::Update_Scene(fTimeDelta);
}

void CStage1::LateUpdate_Scene()
{
    __super::LateUpdate_Scene();
}

void CStage1::Render_Scene()
{
    
}

CStage1* CStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage1* ret = new CStage1(pGraphicDev);

    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Scene()))
    {
        Safe_Release(ret);
        MSG_BOX("Stage1 Create Failed");
        return nullptr;
    }

    return ret;
}

void CStage1::Free()
{
    __super::Free();
}

HRESULT CStage1::Ready_Layer_Player()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_Camera()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_Terrrain()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_Environment()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_Monster()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_InterationObj()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_Effect()
{
    return S_OK;
}

HRESULT CStage1::Ready_Layer_UI()
{
    return S_OK;
}
