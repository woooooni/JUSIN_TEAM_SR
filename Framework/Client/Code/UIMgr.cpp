#include "Export_Function.h"
#include "UIMgr.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
    Free();
}

HRESULT CUIMgr::Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    FAILED_CHECK_RETURN(Add_Icon(_pGraphicDev), E_FAIL);

   // m_pDialog = CUI_Dialog::Create(_pGraphicDev);
    m_pHpBar = CUI_HPBar::Create(_pGraphicDev);
    //m_pShop = CUI_Shop::Create(_pGraphicDev);
   // m_pShortCutKey = CUI_ShortCutKey::Create(_pGraphicDev);
    m_pQuickSlot = CQuickSlot::Create(_pGraphicDev);

    m_pCurrentUI = CInventoryUI::Create(_pGraphicDev);
   // NULL_CHECK_RETURN(m_pDialog, E_FAIL);
    NULL_CHECK_RETURN(m_pHpBar, E_FAIL);
   // NULL_CHECK_RETURN(m_pShop, E_FAIL);
   // NULL_CHECK_RETURN(m_pShortCutKey, E_FAIL);

    FAILED_CHECK_RETURN(Add_Frame(_pGraphicDev), E_FAIL);

    return S_OK;
}

void CUIMgr::Update_UIMgr(const _float& fTimeDelta)
{

    m_vecIcon[HEART]->Update_Object(fTimeDelta);
    m_vecIcon[PLAYERHP_BACK]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBOARD]->Update_Object(fTimeDelta);
    m_vecIcon[QUEST]->Update_Object(fTimeDelta);

    ICONTYPE::PLAYERHP_FRAME, KEYBUTTON_1, KEYBUTTON_2, KEYBUTTON_3, KEYBUTTON_4, KEYBUTTON_L;
   //m_pDialog->Update_Object(fTimeDelta);
    m_pHpBar->Update_Object(fTimeDelta);
   //m_pShop->Update_Object(fTimeDelta);
   //m_pShortCutKey->Update_Object(fTimeDelta);
    m_pQuickSlot->Update_Object(fTimeDelta);

    m_pCurrentUI->Update_Object(fTimeDelta);

    m_vecIcon[PLAYERHP_FRAME]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_1]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_2]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_3]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_4]->Update_Object(fTimeDelta);
    m_vecIcon[KEYBUTTON_L]->Update_Object(fTimeDelta);
}

void CUIMgr::Late_Update_UIMgr()
{
    m_vecIcon[HEART]->LateUpdate_Object();
    m_vecIcon[PLAYERHP_BACK]->LateUpdate_Object();
    m_vecIcon[KEYBOARD]->LateUpdate_Object();
    m_vecIcon[QUEST]->LateUpdate_Object();

   //m_pDialog->LateUpdate_Object();
    m_pHpBar->LateUpdate_Object();
   //m_pShop->LateUpdate_Object();
   //m_pShortCutKey->LateUpdate_Object();
    m_pQuickSlot->LateUpdate_Object();

    m_pCurrentUI->LateUpdate_Object();

    m_vecIcon[PLAYERHP_FRAME]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_1]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_2]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_3]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_4]->LateUpdate_Object();
    m_vecIcon[KEYBUTTON_L]->LateUpdate_Object();
}

void CUIMgr::Render_UIMgr()
{
    m_vecIcon[HEART]->Render_Object();
   // m_vecIcon[PLAYERHP_BACK]->Render_Object();
    m_vecIcon[KEYBOARD]->Render_Object();
    m_vecIcon[QUEST]->Render_Object();

    //m_pDialog->Render_Object();
    m_pHpBar->Render_Object();
   // m_pShop->Render_Object();
    //m_pShortCutKey->Render_Object();
    m_pQuickSlot->Render_Object();

    m_vecIcon[PLAYERHP_BACK]->Render_Object();
    m_vecIcon[PLAYERHP_FRAME]->Render_Object();
    m_vecIcon[KEYBUTTON_1]->Render_Object();
    m_vecIcon[KEYBUTTON_2]->Render_Object();
    m_vecIcon[KEYBUTTON_3]->Render_Object();
    m_vecIcon[KEYBUTTON_4]->Render_Object();
    m_vecIcon[KEYBUTTON_L]->Render_Object();
}

HRESULT CUIMgr::Set_UI(SCENE_TYPE eType)
{
   // enum class SCENE_TYPE { LOADING, TEST, LOGO, TUTORIAL_VILLAGE,
   // MONKEY_FOREST1, MONKEY_FOREST2, MONKEY_VILLAGE, SUNGOLEM_CAVE1, MOON_FOREST1, TOOL, SCENE_END };

    CManagement* pManagement = CManagement::GetInstance();

    return E_NOTIMPL;
}

void CUIMgr::Set_PlayerInfo()
{
   // m_pHpBar->
}

HRESULT CUIMgr::Add_Icon(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    // 먼저 그려져도 되는 Icon들만
    m_vecIcon.reserve(ICONTYPE::ICONTYPE_END);

    CIcon* pHeart = CIcon::Create(_pGraphicDev, ICONTYPE::HEART);
    m_vecIcon.push_back(pHeart);

    CIcon* pHpBack = CIcon::Create(_pGraphicDev, ICONTYPE::PLAYERHP_BACK);
    m_vecIcon.push_back(pHeart);

    CIcon* pKeyboardIcon = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBOARD);
    m_vecIcon.push_back(pKeyboardIcon);

    CIcon* pQuestIcon = CIcon::Create(_pGraphicDev, ICONTYPE::QUEST);
    m_vecIcon.push_back(pQuestIcon);

    return S_OK;
}

HRESULT CUIMgr::Add_Frame(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    // 나중에 그려져야하는 Icon

    CIcon* pHpFrame = CIcon::Create(_pGraphicDev, ICONTYPE::PLAYERHP_FRAME);
    m_vecIcon.push_back(pHpFrame);

    CIcon* pButton1 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_1);
    m_vecIcon.push_back(pButton1);

    CIcon* pButton2 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_2);
    m_vecIcon.push_back(pButton2);

    CIcon* pButton3 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_3);
    m_vecIcon.push_back(pButton3);

    CIcon* pButton4 = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_4);
    m_vecIcon.push_back(pButton4);

    CIcon* pButtonL = CIcon::Create(_pGraphicDev, ICONTYPE::KEYBUTTON_L);
    m_vecIcon.push_back(pButtonL);

    return S_OK;
}

void CUIMgr::Free()
{
    // TODO
}
