#include "UI_Fade.h"
#include "Export_Function.h"

CUI_Fade::CUI_Fade(LPDIRECT3DDEVICE9 pGraphicDev) 
    : CUI(pGraphicDev)
    , m_bIsFadeOut(false)
    , m_bIsComplete(true)
    , m_fFadeTime(0.f)
    , m_bIsWhite(true)
    , m_fAlpha(0.f)

{
    m_iAlpha = 0;
}

CUI_Fade::CUI_Fade(const CUI_Fade& rhs) : CUI(rhs)
{
}

CUI_Fade::~CUI_Fade()
{
}

HRESULT CUI_Fade::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

    return S_OK;
}

_int CUI_Fade::Update_Object(const _float& fTimeDelta)
{
    if (m_bIsComplete)
        return 0;

    if (m_bIsFadeOut && m_fAlpha > 0.f)
    {
        m_fAlpha -= fTimeDelta / m_fFadeTime;

        if (m_fAlpha < 0.f)
            m_fAlpha = 0.f;
    }
    else if (m_bIsFadeOut)
    {

        m_bIsFadeOut = false;
        m_bIsComplete = true;
    }
    else if (!m_bIsFadeOut && m_fAlpha < 1.f)
    {
        m_fAlpha += fTimeDelta / m_fFadeTime;
        if (m_fAlpha > 1.f)
            m_fAlpha = 1.f;

    }
    else if (!m_bIsFadeOut)
    {
        m_bIsFadeOut = true;
        m_bIsComplete = true;
    }



    return __super::Update_Object(fTimeDelta);
}

void CUI_Fade::LateUpdate_Object(void)
{
    Add_RenderGroup(RENDER_UI, this);
    __super::LateUpdate_Object();
}

void CUI_Fade::Render_Object(void)
{
    Ready_TransWorld();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((_uint)(255.f * m_fAlpha), 255, 255, 255));
    m_pTextureCom->Set_Idx((_uint)m_bIsWhite);
    m_pTextureCom->Render_Texture();
    __super::Render_Object();
    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

}

CUI_Fade* CUI_Fade::Create(LPDIRECT3DDEVICE9 p_GraphicDev)
{
    CUI_Fade* pInstance = new CUI_Fade(p_GraphicDev);

    NULL_CHECK_RETURN(pInstance, nullptr);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CUI_Fade FAILED");
        return nullptr;
    }
    pInstance->m_tInfo.fX = WINCX / 2;
    pInstance->m_tInfo.fY = WINCY / 2;
    pInstance->m_tInfo.fCX = WINCX;
    pInstance->m_tInfo.fCY = WINCY;


    return pInstance;
}

HRESULT CUI_Fade::Ready_Component()
{
    CComponent* pComponent = nullptr;

    D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
    D3DXMatrixIdentity(&m_matView);

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Veil"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(this);
    m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);



    return S_OK;
}
