#include "..\Header\Scene_Loading.h"
#include "Export_Function.h"
#include "../Include/stdafx.h"
#include "Loading.h"
#include "UI_Shop.h"
#include "UI_LoadingOgu.h"
#include "UI_LoadingBackGround.h"

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextSceneType)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::LOADING)
	, m_eNextScene(_eNextSceneType)
	, m_pLoading(nullptr)
{
	
}

CScene_Loading::~CScene_Loading()
{
}

HRESULT CScene_Loading::Ready_Scene()
{
	__super::Ready_AllLayer();

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pGraphicDev->SetViewport(&vp);

	MATERIAL.Set_Material(MATERIAL.material, { 1.f, 1.f, 1.f, 0.f });
	D3DMATERIAL9 mater = MATERIAL.material;
	FAILED_CHECK(m_pGraphicDev->SetMaterial(&MATERIAL.material));

	Ready_Layer_UI();

	m_pLoading = CLoading::Create(m_pGraphicDev, m_eNextScene);

	return S_OK;
}

Engine::_int CScene_Loading::Update_Scene(const _float& fTimeDelta)
{
	_int iResult = __super::Update_Scene(fTimeDelta);
	return iResult;
}

void CScene_Loading::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CScene_Loading::Render_Scene()
{
	if (true == m_pLoading->Get_Finish())
		Engine::Reserve_SceneChange(m_pLoading->Get_Scene());
}

void CScene_Loading::Free()
{
	Safe_Release(m_pLoading);
	__super::Free();
}

CScene_Loading* CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene)
{
	CScene_Loading* pInstance = new CScene_Loading(pGraphicDev, _eNextScene);
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Loading Scene Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CScene_Loading::Ready_Prototype()
{
	return S_OK;
}



HRESULT CScene_Loading::Ready_Layer_Player()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::PLAYER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Camera()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::CAMERA];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CLayer* pPlayerLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Terrrain()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::TERRAIN];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Environment()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::ENVIRONMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_Effect()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::EFFECT];

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_UI()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUI_LoadingOgu* pUIOgu = CUI_LoadingOgu::Create(m_pGraphicDev);
	CUI_LoadingBackGround* pUIBackGround = CUI_LoadingBackGround::Create(m_pGraphicDev);

	pLayer->Add_GameObject(L"Ogu_UI", pUIOgu);
	pLayer->Add_GameObject(L"BackGround", pUIBackGround);

	pLayer->Ready_Layer();

	return S_OK;
}

