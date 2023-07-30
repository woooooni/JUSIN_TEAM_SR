#include "..\Header\MainApp.h"
#include "../Include/stdafx.h"
#include "Engine_Define.h"
#include "Export_Function.h"

#include "ImGuiMgr.h"
#include "GameMgr.h"
#include "LightMgr.h"
#include "QuestMgr.h"
#include "UIMgr.h"
#include "InventoryMgr.h"
#include "QuestMgr.h"


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
#include "TurretBullet.h"

#include "Scene_Loading.h"
#include <time.h>

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
	FAILED_CHECK_RETURN(Ready_Manager(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Pool(), E_FAIL);

	srand(unsigned(time(NULL)));

	return S_OK;
}

int CMainApp::Update_MainApp(const float & fTimeDelta)
{
	
	Engine::Update_InputDev();
	Engine::Update_PickingMgr();
	Engine::Update_KeyMgr();
	

	NULL_CHECK_RETURN(m_pManagementClass, -1);
	m_pManagementClass->Update_Scene(fTimeDelta);
	CQuestMgr::GetInstance()->Update_QuestMgr(fTimeDelta);

	
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	NULL_CHECK(m_pManagementClass);
	m_pManagementClass->LateUpdate_Scene();
	Engine::Update_Collision();
	Engine::LateUpdate_LightMgr();

}

void CMainApp::Render_MainApp()
{

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));


	m_pManagementClass->Render_Scene(m_pGraphicDev);



	Engine::Update_EventMgr(0.f);
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
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

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

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	

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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shader", CShader::Create(m_pGraphicDev, L"../Bin/ShaderFiles/LightShader.hlsl")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Main", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Banner.png")), E_FAIL);

	// 초반 로딩을 위한 텍스처 로딩.
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_OguLoading", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Loading/Ogu/Loading_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_LoadingBackGround", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Loading/BackGround/BackGround.png")), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_SoundMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_EventMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_PickingMgr(pGraphicDev, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_KeyMgr(pGraphicDev, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_CollisionMgr(pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(CEventMgr::GetInstance()->Ready_EventMgr(), E_FAIL);
	FAILED_CHECK_RETURN(CInventoryMgr::GetInstance()->Ready_InvenMgr(pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(CQuestMgr::GetInstance()->Ready_QuestMgr(), E_FAIL);
	FAILED_CHECK_RETURN(CLightMgr::GetInstance()->Ready_LightMgr(pGraphicDev), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Proto_Object(LPDIRECT3DDEVICE9 pGraphicDev)
{

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
	
	CEventMgr::GetInstance()->DestroyInstance();
	CInventoryMgr::GetInstance()->DestroyInstance();
	CQuestMgr::GetInstance()->DestroyInstance();
	CUIMgr::GetInstance()->DestroyInstance();

	Engine::Release_Utility();
	Engine::Release_System();
}
