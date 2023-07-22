#include "..\Header\Scene_SunGolemCave1.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "Portal.h"
#include "UIMgr.h"
#include "SunGollem.h"
#include	"Catapult.h"

CScene_SunGolemCave1::CScene_SunGolemCave1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::SUNGOLEM_CAVE1)
{

}

CScene_SunGolemCave1::~CScene_SunGolemCave1()
{
}

HRESULT CScene_SunGolemCave1::Ready_Scene()
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

_int CScene_SunGolemCave1::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_SunGolemCave1::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_SunGolemCave1::Render_Scene()
{
	RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));

	CUIMgr::GetInstance()->Render_UIMgr();
}

HRESULT CScene_SunGolemCave1::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	CUIMgr::GetInstance()->Ready_UIMgr(m_pGraphicDev);
	
	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 10.f;
	vStartPos.z = -0.5f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);

	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Environment()
{

	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MOON_FOREST1);
	_vec3 vPortalPos = _vec3(9.f, 0.5f, 21.f);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Monster()
{
	CSunGollem* pSunGollem = CSunGollem::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSunGollem, E_FAIL);
	m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"SunGollem", pSunGollem);

	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_InterationObj()
{
	CCatapult* pCat = CCatapult::Create(m_pGraphicDev, 0, { 10.f, 0.f, 5.f });
	NULL_CHECK_RETURN(pCat, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Catapult", pCat);


	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_SunGolemCave1::Ready_Layer_UI()
{
	return S_OK;
}

CScene_SunGolemCave1* CScene_SunGolemCave1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_SunGolemCave1* pInstance = new CScene_SunGolemCave1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_SunGolemCave1::Free()
{
	__super::Free();
}
