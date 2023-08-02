#include "..\Header\Scene_TutorialVillage.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Terrain.h"
#include "Npc_Cow.h"
#include "Npc_Sheep.h"
#include "Portal.h"
#include "UIMgr.h"
#include "LightFlower.h"
#include "DesertRhino.h"
#include "MothMage.h"
#include "Door.h"
#include "DefaultItem.h"
#include "UI_NewItem.h"
#include "Npc_OguMom.h"
#include "TrashFast.h"
#include "UI_MapName.h"
#include "Cupa.h"
#include "RabitObj.h"
#include "Turret.h"
#include "DrawingEnter.h"
#include "UIMgr.h"
#include "BarrelBomb.h"
#include "BreakObj.h"
#include "Particle_MapCircle.h"

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

	D3DLIGHT9 tLight;
	tLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tLight.Direction = { 0.0f, -1.0f, 1.0f };
	tLight.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	tLight.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	tLight.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	CLightMgr::GetInstance()->Reset_Light();
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Set_LightInfo(tLight);

	return S_OK;
}

_int CScene_TutorialVillage::Update_Scene(const _float& fTimeDelta)
{
	CUIMgr::GetInstance()->Update_UIMgr(fTimeDelta);

	__super::Update_Scene(fTimeDelta);
	return S_OK;
}

void CScene_TutorialVillage::LateUpdate_Scene()
{
	CUIMgr::GetInstance()->Late_Update_UIMgr();

	__super::LateUpdate_Scene();
}

void CScene_TutorialVillage::Render_Scene()
{
	/* RECT rcPos = { WINCX / 2 - 10.f, 0,  WINCX / 2 + 10.f, 200.f };
	_vec3 vPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	wstring strPos = L"X : " + to_wstring(vPos.x) + L"\nY : " + to_wstring(vPos.y) + L"\nZ : " + to_wstring(vPos.z);
	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
		strPos.c_str(), INT(strPos.size()), &rcPos, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(100, 0, 0, 0));*/


}

void CScene_TutorialVillage::Enter_Scene()
{
	Stop_Sound(CHANNELID::SOUND_BGM);
	Play_BGM(L"BGM_0_Null.wav", 0.5f);


	CUI_Shop* pShop = CUIMgr::GetInstance()->Get_ShopUI();
	pShop->Add_Item(CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HAT_LIGHT), SHOPITEMTYPE::UISHOP_LEAF);
	pShop->Add_Item(CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HAT_MASK), SHOPITEMTYPE::UISHOP_CLOTH);
	pShop->Add_Item(CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HAT_MISSLE), SHOPITEMTYPE::UISHOP_BRANCH);




}

void CScene_TutorialVillage::Exit_Scene()
{
	
}

HRESULT CScene_TutorialVillage::Ready_Prototype()
{
	FAILED_CHECK_RETURN(CGameMgr::GetInstance()->Ready_GameMgr(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(CUIMgr::GetInstance()->Ready_UIMgr(m_pGraphicDev), E_FAIL);
	
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Player()
{
	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	m_mapLayer[LAYER_TYPE::PLAYER]->Add_GameObject(L"Player", pPlayer);
	m_mapLayer[LAYER_TYPE::PLAYER]->Ready_Layer();

	_vec3 vStartPos;
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vStartPos);
	vStartPos.x = 5.5f;
	vStartPos.z = 6.5f;
	pPlayer->Get_TransformCom()->Set_Info(INFO_POS, &vStartPos);

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
	CNpc_Sheep* pNpcSheep = CNpc_Sheep::Create(m_pGraphicDev);
	CNpc_Cow* pNpcCow = CNpc_Cow::Create(m_pGraphicDev);

	CPortal* pPortal = CPortal::Create(m_pGraphicDev, SCENE_TYPE::MONKEY_FOREST1);

	//CLightFlower* pFlower = CLightFlower::Create(m_pGraphicDev, nullptr);

	_vec3 vSheepPos = _vec3(20.5f, 0.5f, 13.5f);
	_vec3 vCowPos = _vec3(24.f, 0.5f, 13.f);
	_vec3 vPortalPos = _vec3(51.8f, 0.5f, 46.5f);

	pNpcSheep->Get_TransformCom()->Set_Info(INFO_POS, &vSheepPos);
	pNpcCow->Get_TransformCom()->Set_Info(INFO_POS, &vCowPos);
	pPortal->Get_TransformCom()->Set_Info(INFO_POS, &vPortalPos);

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Npc_Sheep", pNpcSheep);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Npc_Cow", pNpcCow);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"NextPortal", pPortal);

	CNpc_OguMom* pMom = CNpc_OguMom::Create(m_pGraphicDev);
	_vec3 vMomPos = _vec3(12.f, 0.5f, 12.f);
	pMom->Get_TransformCom()->Set_Info(INFO_POS, &vMomPos);
	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"Npc_OguMom", pMom);
	

	for (_uint i = 0; i < 300; ++i)
	{
		CParticle_MapCircle* pParticle = CParticle_MapCircle::Create(Engine::Get_Device());
		NULL_CHECK_RETURN(pParticle, E_FAIL);
		pParticle->Random_Particle(_vec3(30.0f, 10.0f, 30.0f), 300, 255, 255, 255, 40);
		m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Add_GameObject(L"MapCircle", pParticle);
	}
	

	m_mapLayer[LAYER_TYPE::ENVIRONMENT]->Ready_Layer();

	

	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_InterationObj()
{
	CBarrelBomb* barrel = CBarrelBomb::Create(m_pGraphicDev, { -1, 0, -1 });
	NULL_CHECK_RETURN(barrel, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Barrel", barrel);

	CBreakObj* breaj = CBreakObj::Create(m_pGraphicDev, { -1, 0, -2 });
	NULL_CHECK_RETURN(breaj, E_FAIL);
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Break", breaj);

	CDefaultItem* def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_SMALL);
	def->Get_TransformCom()->Set_Pos(&_vec3(11, 0, 5));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_MIDDLE);
	def->Get_TransformCom()->Set_Pos(&_vec3(12, 0, 4));

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_BIG);
	def->Get_TransformCom()->Set_Pos(&_vec3(13, 0, 3));

	//TEST
	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_BIG);
	def->Get_TransformCom()->Set_Pos(&_vec3(14, 0, 2));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_MIDDLE);
	def->Get_TransformCom()->Set_Pos(&_vec3(13, 0, 2));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_MIDDLE);
	def->Get_TransformCom()->Set_Pos(&_vec3(5, 0, 2));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_BIG);
	def->Get_TransformCom()->Set_Pos(&_vec3(10, 0, 2));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::HP_SMALL);
	def->Get_TransformCom()->Set_Pos(&_vec3(12, 0, 2));
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);
	//
	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::SPEED_SMALL);
	def->Get_TransformCom()->Set_Pos(&_vec3(14, 0, 8));

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);

	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::SPEED_MIDDLE);
	def->Get_TransformCom()->Set_Pos(&_vec3(15, 0, 5));

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);


	def = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::SPEED_BIG);
	def->Get_TransformCom()->Set_Pos(&_vec3(16, 0, 5));

	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Add_GameObject(L"Item", def);


	m_mapLayer[LAYER_TYPE::INTERACTION_OBJ]->Ready_Layer();
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_UI()
{
	CUI_MapName* pMapName = CUI_MapName::Create(m_pGraphicDev, SCENE_TYPE::TUTORIAL_VILLAGE);
	m_mapLayer[LAYER_TYPE::UI]->Add_GameObject(L"MapName", pMapName);

	m_mapLayer[LAYER_TYPE::UI]->Ready_Layer();

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
