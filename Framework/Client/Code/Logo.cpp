#include "..\Header\Logo.h"
#include "Export_Function.h"
#include "../Include/stdafx.h"
#include	"Loading.h"
#include	"UI_Shop.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::LOADING)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
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

	m_pLoading = CLoading::Create(m_pGraphicDev, SCENE_TYPE::TOOL);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	
}

void CLogo::Free()
{
	__super::Free();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CLogo::Ready_Prototype()
{
	return S_OK;
}



HRESULT CLogo::Ready_Layer_Player()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::PLAYER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Camera()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::CAMERA];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CLayer* pPlayerLayer = m_mapLayer[LAYER_TYPE::PLAYER];

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Terrrain()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::TERRAIN];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::ENVIRONMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Monster()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::MONSTER];
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_InterationObj()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::INTERACTION_OBJ];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Effect()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::EFFECT];

	pLayer->Ready_Layer();

	return S_OK;
}

HRESULT CLogo::Ready_Layer_UI()
{
	Engine::CLayer* pLayer = m_mapLayer[LAYER_TYPE::UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Ready_Layer();

	return S_OK;
}

