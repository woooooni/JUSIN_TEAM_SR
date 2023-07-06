#include "../Include/stdafx.h"
#include "..\Header\Scene_Tool.h"
#include "Export_Function.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera.h"
#include "CameraMgr.h"
#include "ImGuiMgr.h"


CScene_Tool::CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev, SCENE_TYPE::TOOL)
{
}

CScene_Tool::~CScene_Tool()
{
}

HRESULT CScene_Tool::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYER_TYPE::ENVIRONMENT), E_FAIL);

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINCX;
	vp.Height = WINCY;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pGraphicDev->SetViewport(&vp);

	return S_OK;
}

Engine::_int CScene_Tool::Update_Scene(const _float& fTimeDelta)
{

	__super::Update_Scene(fTimeDelta);
	CImGuiMgr::GetInstance()->Update_ImGui();
	return 0;
}

void CScene_Tool::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
	
}

void CScene_Tool::Render_Scene()
{
	CImGuiMgr::GetInstance()->Render_ImGui();
}

void CScene_Tool::Free()
{
	__super::Free();
}

CScene_Tool* CScene_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Tool* pInstance = new CScene_Tool(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CScene_Tool::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Environment(LAYER_TYPE _eType)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pPlayer), E_FAIL);

	// Camera
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pCamera), E_FAIL);

	// UI
	//CUI* pUI = CUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pUI, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pUI), E_FAIL);

	pCamera->Set_TargetObj(pPlayer);

	m_mapLayer.insert({ _eType, pLayer });

	return S_OK;
}
