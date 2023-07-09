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
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
{
}

CScene_Tool::~CScene_Tool()
{
}

HRESULT CScene_Tool::Ready_Scene()
{
	__super::Ready_AllLayer();
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

	CImGuiMgr::GetInstance()->SetToolScene(this);    

	return S_OK;
}

Engine::_int CScene_Tool::Update_Scene(const _float& fTimeDelta)
{
	_int		iResult = 0;
	for (auto& iter : m_mapLayer)
	{
		if (iter.first == LAYER_TYPE::MONSTER)
		{
			for (_uint i = 0; i < iter.second->Get_GameObjectVec().size(); ++i)
				iter.second->Get_GameObjectVec()[i]->Set_Billboard();

			iResult = iter.second->Update_Layer(0.f);
		}	
		else
			iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
	CImGuiMgr::GetInstance()->Update_ImGui(fTimeDelta);
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

void CScene_Tool::Save_Data()
{
}

void CScene_Tool::Load_Data()
{
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
	Engine::CLayer* pLayerCamera = m_mapLayer.find(LAYER_TYPE::CAMERA)->second;
	Engine::CLayer* pLayerPlayer = m_mapLayer.find(LAYER_TYPE::PLAYER)->second;
	Engine::CLayer* pLayerEnv = m_mapLayer.find(LAYER_TYPE::ENVIRONMENT)->second;
	Engine::CLayer* pLayerTerrain = m_mapLayer.find(LAYER_TYPE::TERRAIN)->second;

	NULL_CHECK_RETURN(pLayerPlayer, E_FAIL);
	NULL_CHECK_RETURN(pLayerEnv, E_FAIL);
	NULL_CHECK_RETURN(pLayerTerrain, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//Terrain
	CTerrain* pTerrain = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayerTerrain->Add_GameObject(L"Terrain", pTerrain), E_FAIL);

	// Player
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayerPlayer->Add_GameObject(L"Player", pPlayer), E_FAIL);
	m_pPlayer = pPlayer;

	// Camera
	Engine::CCamera * pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 1.f, 1000.f);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayerCamera->Add_GameObject(L"MainCamera", pCamera), E_FAIL);
	m_pCamera = pCamera;


	CImGuiMgr::GetInstance()->Set_Target(pPlayer);
	pCamera->Set_CameraState(CAMERA_STATE::TOOL);
	pCamera->Set_TargetObj(pPlayer);

	return S_OK;
}


