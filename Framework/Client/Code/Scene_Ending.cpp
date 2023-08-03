#include "Scene_Ending.h"
#include "UI_LoadingBackGround.h"
#include "UIMgr.h"
#include "UI_Ending.h"
#include "GameMgr.h"

CScene_Ending::CScene_Ending(LPDIRECT3DDEVICE9 pGraphicDev) 
    : Engine::CScene(pGraphicDev, SCENE_TYPE::ENDING)
{
}

CScene_Ending::~CScene_Ending()
{
}

HRESULT CScene_Ending::Ready_Scene()
{

    __super::Ready_AllLayer();

    m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", CGameMgr::GetInstance()->Get_Player());
    m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

    _vec3 vPos;
    CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
    vPos.x = 58.5f;
    vPos.z = 2.f;
    CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Set_Info(INFO_POS, &vPos);


    D3DVIEWPORT9 vp;
    vp.X = 0;
    vp.Y = 0;
    vp.Width = WINCX;
    vp.Height = WINCY;
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;

    m_pGraphicDev->SetViewport(&vp);

    Ready_Layer_UI();



    return S_OK;
}

_int CScene_Ending::Update_Scene(const _float& fTimeDelta)
{
    CUIMgr::GetInstance()->Get_Ending()->Update_Object(fTimeDelta);
    return __super::Update_Scene(fTimeDelta);
}

void CScene_Ending::LateUpdate_Scene()
{
    __super::LateUpdate_Scene();
}

void CScene_Ending::Render_Scene()
{
}

HRESULT CScene_Ending::Ready_Prototype()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Player()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Camera()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Terrrain()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Environment()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Monster()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_InterationObj()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_Effect()
{
    return S_OK;
}

HRESULT CScene_Ending::Ready_Layer_UI()
{
    Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    pLayer->Ready_Layer();

    return S_OK;

}

CScene_Ending* CScene_Ending::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CScene_Ending* pInstance = new CScene_Ending(pGraphicDev);
    if (FAILED(pInstance->Ready_Scene()))
    {
        Safe_Release(pInstance);

        MSG_BOX("Loading Scene Create Failed");
        return nullptr;
    }

    return pInstance;

}

void CScene_Ending::Free()
{
    __super::Free();
}
