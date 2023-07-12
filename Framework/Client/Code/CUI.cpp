#include "Export_Function.h"
#include "CUI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_UI, OBJ_ID::UI)
{
	ZeroMemory(&m_tInfo, sizeof(UI_INFO));
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
	m_tInfo = rhs.m_tInfo;
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);

	m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);

	m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

	return S_OK;
}


_int CUI::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);
	CGameObject::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUI::Render_Object(void)
{
	CGameObject::Render_Object();
}

void CUI::Debug_Input()
{
	if (KEY_TAP(KEY::UP_ARROW))
	{
		m_tInfo.fY -= 1.f;
	}

	if (KEY_TAP(KEY::DOWN_ARROW))
	{
		m_tInfo.fY += 1.f;
	}

	if (KEY_TAP(KEY::LEFT_ARROW))
	{
		m_tInfo.fX -= 1.f;
	}

	if (KEY_TAP(KEY::RIGHT_ARROW))
	{
		m_tInfo.fX += 1.f;
	}
}

void CUI::Free()
{
	CGameObject::Free();
}
