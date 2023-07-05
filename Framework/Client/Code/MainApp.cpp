#include "..\Header\MainApp.h"

#include "Engine_Define.h"
#include "Export_Function.h"
#include "Logo.h"

CMainApp::CMainApp() : m_pGraphicDevClass(nullptr), m_pManagementClass(nullptr)
{
	
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Default_SamplerState(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Default_RenderState(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto_Component(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
	FAILED_CHECK_RETURN(Ready_PickingMgr(m_pGraphicDev, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Ready_KeyMgr(m_pGraphicDev, g_hWnd), E_FAIL);

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
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	m_pManagementClass->Render_Scene(m_pGraphicDev);
	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pGraphicDevClass), E_FAIL);
	m_pGraphicDevClass->AddRef();

	(*ppGraphicDev) = m_pGraphicDevClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();


	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInstance, g_hWnd), E_FAIL);
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

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

// 프로토타입 컴포넌트 추가.
HRESULT CMainApp::Ready_Proto_Component(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCube", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, 257, 257, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Animator", CAnimator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxCollider", CBoxCollider::Create(m_pGraphicDev)), E_FAIL);


	// Initialize Textures
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain_Test.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player_Test/Player_%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/UI/Test.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Stone", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/IA_Sprite/Sprite_StonePushable.png")), E_FAIL);



	// Player
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/Down/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/Up/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/Left/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/Right/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/LeftUp/Player_Idle_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/LeftDown/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/RightUp/Player_Idle_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Idle_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Idle/RightDown/Player_Idle_%d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/Down/Player_Walk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/Up/Player_Walk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/Left/Player_Walk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/Right/Player_Walk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/LeftUp/Player_Walk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/LeftDown/Player_Walk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/RightUp/Player_Walk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Walk_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Walk/RightDown/Player_Walk_%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/Down/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/Up/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/Left/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/Right/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/LeftUp/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/LeftDown/Player_Run_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/RightUp/Player_Run_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Run_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Run/RightDown/Player_Run_%d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/Down/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/Up/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/Left/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/Right/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/LeftUp/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/LeftDown/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/RightUp/Player_Rolling_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Rolling_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Rolling/RightDown/Player_Rolling_%d.png", 6)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/Down/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/Up/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/Left/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/Right/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/LeftUp/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/LeftDown/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/RightUp/Player_Jump_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Jump_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Jump/RightDown/Player_Jump_%d.png", 11)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/Down/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/Up/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/Left/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/Right/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/LeftUp/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/LeftDown/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/RightUp/Player_Swing_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Swing_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Swing/RightDown/Player_Swing_%d.png", 9)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/Down/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/Up/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/Left/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/Right/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/LeftUp/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/LeftDown/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/RightUp/Player_Hit_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Hit_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Hit/RightDown/Player_Hit_%d.png", 3)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/Down/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/Up/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/Left/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/Right/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/LeftUp/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/LeftDown/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/RightUp/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftUp_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftUp/RightDown/Player_Lift_%d.png", 8)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/Down/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/Up/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/Left/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/Right/Player_Lift_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/LeftUp/Player_Lift_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/LeftDown/Player_Lift_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/RightUp/Player_Lift_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftDown_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftDown/RightDown/Player_Lift_%d.png", 7)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/Down/Player_LiftWalk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/Up/Player_LiftWalk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/Left/Player_LiftWalk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/Right/Player_LiftWalk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/LeftUp/Player_LiftWalk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/LeftDown/Player_LiftWalk_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/RightUp/Player_LiftWalk_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_LiftWalk_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_LiftWalk/RightDown/Player_LiftWalk_%d.png", 9)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/Down/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/Up/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/Left/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/Right/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/LeftUp/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/LeftDown/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/RightUp/Player_Lift_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Lift_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Lift/RightDown/Player_Lift_%d.png", 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_NearPlant", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/IA_Sprite/Sprite_JungleBrackenReact_%d.png", 15)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_BlueBeatle_Idle_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/BlueBeatle/Idle/Down/BlueBeatle_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_BlueBeatle_Move_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/BlueBeatle/Idle/Down/BlueBeatle_%d.png", 3)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/Down/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/Up/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/Left/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/Right/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/LeftUp/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/LeftDown/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/RightDown/TrashBig_%d.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Idle_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Idle/RightUp/TrashBig_%d.png", 1)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,		L"../Bin/Resource/Texture/Monster/TrashBig/Movement/Down/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,		L"../Bin/Resource/Texture/Monster/TrashBig/Movement/Up/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,		L"../Bin/Resource/Texture/Monster/TrashBig/Movement/Left/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,	L"../Bin/Resource/Texture/Monster/TrashBig/Movement/Right/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_LeftUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,	L"../Bin/Resource/Texture/Monster/TrashBig/Movement/LeftUp/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_LeftDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/TrashBig/Movement/LeftDown/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_RightDown", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,L"../Bin/Resource/Texture/Monster/TrashBig/Movement/RightDown/TrashBig_%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_TrashBig_Move_RightUp", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL,	L"../Bin/Resource/Texture/Monster/TrashBig/Movement/RightUp/TrashBig_%d.png", 5)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Push_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Push/Down/Player_Push_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Push_Up", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Push/Up/Player_Push_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Push_Left", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Push/Left/Player_Push_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Push_Right", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Push/Right/Player_Push_%d.png", 10)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_Down", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_Down/Player_Down_%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texture_Player_GameOver", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Player_GameOver/Player_GameOver_%d.png", 25)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GrabStone", CTexture::Create(m_pGraphicDev, TEXTUREID::TEX_NORMAL, L"../Bin/Resource/IA_Sprite/Sprite_NaturalObjects_StoneLiftable.png")), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	Engine::CScene*		pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagementClass), E_FAIL);
	(*ppManagementClass)->AddRef();

	FAILED_CHECK_RETURN((*ppManagementClass)->Set_Scene(pScene), E_FAIL);

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
