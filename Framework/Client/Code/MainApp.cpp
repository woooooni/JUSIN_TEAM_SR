#include "..\Header\MainApp.h"
#include "../Include/stdafx.h"
#include "Engine_Define.h"
#include "Export_Function.h"

#include "ImGuiMgr.h"
#include "GameMgr.h"
#include "LightMgr.h"

#include "Player_Bullet_Lightning.h"
#include "Player_Bullet_Bomb.h"
#include "Particle_FixedLeaf.h"
#include "Particle_MovingLeaf.h"
#include "Effect_Leaf.h"
#include "Scene_Test.h"

#include "Pool.h"
#include "Effect_Shadow.h"
#include "Scene_TutorialVillage.h"

#include "CParticle_Stone.h"
#include "Effect_DieSmoke.h"
#include "Effect_Explosion.h"
#include "Effect_LightningGround.h"
#include "Effect_GolemFist.h"
#include "Effect_Item.h"
#include "Effect_Block.h"
#include "Effect_Hit.h"

#include "Scene_Loading.h"

CMainApp::CMainApp() : m_pGraphicDevClass(nullptr), m_pManagementClass(nullptr), m_pGraphicDev(nullptr)
{
	
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	// _CrtSetBreakAlloc(3474);//문제 발생 시, 메모리 릭 부분에 중단점
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Default_SamplerState(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Default_RenderState(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto_Component(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto_Event(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Manager(m_pGraphicDev), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Pool(), E_FAIL);

	return S_OK;
}

int CMainApp::Update_MainApp(const float & fTimeDelta)
{
	Engine::Update_InputDev();
	Engine::Update_PickingMgr();
	Engine::Update_KeyMgr();

	NULL_CHECK_RETURN(m_pManagementClass, -1);
	m_pManagementClass->Update_Scene(fTimeDelta);

	
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	NULL_CHECK(m_pManagementClass);
	m_pManagementClass->LateUpdate_Scene();
	Engine::Update_Collision();
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	m_pManagementClass->Render_Scene(m_pGraphicDev);

	// Engine::Update_EventMgr(0.f);
	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pGraphicDevClass), E_FAIL);
	m_pGraphicDevClass->AddRef();

	(*ppGraphicDev) = m_pGraphicDevClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();


	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInstance, g_hWnd), E_FAIL);
	

	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	return S_OK;
}

HRESULT CMainApp::Ready_Default_SamplerState()
{
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	return S_OK;
}

HRESULT CMainApp::Ready_Default_RenderState()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	// ===================================================
	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Direction = { 0.f, -1.f, 1.f };
	// ===================================================

	// ===================================================
	D3DMATERIAL9 tMtrl = MATERIAL.Get_Meretial({ 1.f, 1.f, 1.f, 1.f });
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };

	// ===================================================
	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);
	m_pGraphicDev->SetMaterial(&tMtrl);
	

	return S_OK;
}

// 프로토타입 컴포넌트 추가.
HRESULT CMainApp::Ready_Proto_Component(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCube", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCubeCol", CRcCubeCol::Create(m_pGraphicDev)), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, 257, 257, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LightPuzzle1", CRcPuzzleBuff::Create(m_pGraphicDev, 5, 6, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Animator", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxCollider", CBoxCollider::Create(m_pGraphicDev)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RigidBody", CRigidBody::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Main", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Banner.png")), E_FAIL);

	// 초반 로딩을 위한 텍스처 로딩.
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_OguLoading", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Loading/Ogu/Loading_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_LoadingBackGround", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Loading/BackGround/BackGround.png")), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_EventMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_PickingMgr(pGraphicDev, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Ready_KeyMgr(pGraphicDev, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Ready_CollisionMgr(pGraphicDev), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Proto_Object(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//CPool<CPlayer_Bullet_Lightning>::Ready_Pool(m_pGraphicDev, 200);


	return S_OK;
}


HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagementClass), E_FAIL);
	(*ppManagementClass)->AddRef();

	Engine::CScene*		pScene = nullptr;

	pScene = CScene_Loading::Create(pGraphicDev, SCENE_TYPE::LOGO);


	NULL_CHECK_RETURN(pScene, E_FAIL);
	FAILED_CHECK_RETURN((*ppManagementClass)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Proto_Event()
{
	EVENT* event = new EVENT;

	event->iEventNum = 1;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 2;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;

	event->iEventNum = 3;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 4;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 5;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 6;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 7;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 8;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 9;
	event->m_bIsCanReset = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 10;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 11;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 12;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 13;
	event->lEndKey.push_back(10);
	event->lEndKey.push_back(11);
	event->lEndKey.push_back(12);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 14;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 15;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 16;

	event->lEndKey.push_back(14);
	event->lEndKey.push_back(15);
	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 17;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 18;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 19;

	event->lEndKey.push_back(17);
	event->lEndKey.push_back(18);

	event->m_bIsCheckUpdate = true;


	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 20;

	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 21;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 22;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 23;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 24;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));

	event = new EVENT;
	event->iEventNum = 25;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 26;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));
	event = new EVENT;
	event->iEventNum = 27;
	event->m_bIsCanReset = true;

	FAILED_CHECK(Add_Event(event));


	event = new EVENT;
	event->iEventNum = 28;
	event->lEndKey.push_back(25);
	event->lEndKey.push_back(26);
	event->lEndKey.push_back(27);

	event->m_bIsCheckUpdate = true;

	FAILED_CHECK(Add_Event(event));

	return S_OK;
}

HRESULT CMainApp::Ready_Pool()
{
	

	return S_OK;
}


CMainApp * CMainApp::Create()
{
	CMainApp *	pInstance = new	CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;

		return pInstance;
	}
	
	return pInstance;
}
	
void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pGraphicDevClass);
	Safe_Release(m_pManagementClass);
		
	Engine::Release_Utility();
	Engine::Release_System();
}
