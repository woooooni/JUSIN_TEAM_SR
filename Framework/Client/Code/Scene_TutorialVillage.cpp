#include "..\Header\Scene_TutorialVillage.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"

CScene_TutorialVillage::CScene_TutorialVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::TUTORIAL_VILLAGE)
{
}

CScene_TutorialVillage::~CScene_TutorialVillage()
{
}

HRESULT CScene_TutorialVillage::Ready_Scene()
{
	
	__super::Ready_AllLayer();
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	return S_OK;
}

_int CScene_TutorialVillage::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_TutorialVillage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CScene_TutorialVillage::Render_Scene()
{
	RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));
}

HRESULT CScene_TutorialVillage::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Player()
{
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", CGameMgr::GetInstance()->Get_Player());
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.x = 4.5f;
	vPos.z = 3.5f;

	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);
	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Environment()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_InterationObj()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_UI()
{
	return S_OK;
}

CScene_TutorialVillage* CScene_TutorialVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_TutorialVillage* pInstance = new CScene_TutorialVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_TutorialVillage::Free()
{
	__super::Free();
}
