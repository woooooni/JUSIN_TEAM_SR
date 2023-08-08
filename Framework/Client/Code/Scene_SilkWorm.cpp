#include "..\Header\Scene_SilkWorm.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "Portal.h"
#include "UIMgr.h"
#include "SilkWorm.h"
#include "GameMgr.h"
#include "Npc_NueHero.h"
#include "SkyBox.h"

// Test Header
#include "QuestMgr.h"
#include "Quest_NueHero1.h"

CScene_SilkWorm::CScene_SilkWorm(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::SILK_WORM)
{

}

CScene_SilkWorm::~CScene_SilkWorm()
{
}

HRESULT CScene_SilkWorm::Ready_Scene()
{

	__super::Ready_AllLayer();

	Clear_Event();

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrrain(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_InterationObj(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(), E_FAIL);

	D3DLIGHT9 tLight;
	tLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tLight.Direction = { 0.0f, -1.0f, 1.0f };
	tLight.Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	tLight.Diffuse = { 0.2f, 0.2f, 0.2f, 1.0f };
	tLight.Specular = { 0.2f, 0.2f, 0.2f, 1.0f };

	Stop_Sound(CHANNELID::SOUND_BGM);
	Play_BGM(L"BGM_38_MoonPowerPlant.wav", 0.3f);
	CLightMgr::GetInstance()->Reset_Light();
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Set_LightInfo(tLight);

	return S_OK;
}

_int CScene_SilkWorm::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);
	
	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_SilkWorm::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();
	__super::LateUpdate_Scene();
}

void CScene_SilkWorm::Render_Scene()
{
	/* RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));*/

	CUIMgr::GetInstance()->Render_UIMgr();
}

void CScene_SilkWorm::Enter_Scene()
{

}

void CScene_SilkWorm::Exit_Scene()
{
	Stop_All();
}

HRESULT CScene_SilkWorm::Ready_Prototype()
{
	CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev);
	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 53.f;
	vStartPos.z = 4.0f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Camera()
{
	CCamera* pCamera = Engine::CreateCamera(g_hWnd, m_pGraphicDev, 0.1f, 1000.f);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"MainCamera", pCamera);

	pCamera->Set_TargetObj(m_mapLayer[LAYER_TYPE::PLAYER]->Find_GameObject(L"Player"));
	m_mapLayer[LAYER_TYPE::CAMERA]->Ready_Layer();

	CSkyBox* pSkyBox = CSkyBox::Create(m_pGraphicDev, 3);
	m_mapLayer[LAYER_TYPE::CAMERA]->Add_GameObject(L"Skybox", pSkyBox);

	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Terrrain()
{
	m_mapLayer[LAYER_TYPE::TERRAIN]->Add_GameObject(L"Terrain", CTerrain::Create(m_pGraphicDev));
	m_mapLayer[LAYER_TYPE::TERRAIN]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Environment()
{
	CNpc_NueHero* nue = CNpc_NueHero::Create(m_pGraphicDev);
	nue->Get_TransformCom()->Set_Pos(&_vec3(53.f, 0.5f, 26.f));
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Nue_NPC", nue);


	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Monster()
{
	//CSilkWorm* pSilkWorm = CSilkWorm::Create(m_pGraphicDev);
	//_vec3 vPos = _vec3(53.f, 0.5f, 26.f);
	//pSilkWorm->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

	//m_mapLayer[LAYER_TYPE::MONSTER]->Add_GameObject(L"SilkWorm", pSilkWorm);

	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_InterationObj()
{
	CQuestMgr::GetInstance()->Add_Quest(new CQuest_NueHero1);
	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_SilkWorm::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::SILK_WORM);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();

	return S_OK;
}

CScene_SilkWorm* CScene_SilkWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_SilkWorm* pInstance = new CScene_SilkWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SilkWorm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_SilkWorm::Free()
{
	__super::Free();
}
