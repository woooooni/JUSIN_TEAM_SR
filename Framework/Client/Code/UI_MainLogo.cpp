#include "../Header/UI_MainLogo.h"
#include "Export_Function.h"

CUI_MainLogo::CUI_MainLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_fAlpha(255.f)
	, m_bIncreaseAlpha(false)
{
}

CUI_MainLogo::CUI_MainLogo(const CUI_MainLogo& rhs)
	: CUI(rhs)
{
}

CUI_MainLogo::~CUI_MainLogo()
{
}

HRESULT CUI_MainLogo::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = _float(WINCX / 2.f);
	m_tInfo.fY = _float(WINCY / 2.f);

	m_tInfo.fCX = _float(WINCX);
	m_tInfo.fCY = _float(WINCY);

	_float fRatio = m_tInfo.fCY / m_tInfo.fCX;
	m_pTransformCom->Set_Scale(_vec3(m_tInfo.fCX, m_tInfo.fCY, 0.5f));
	m_pTextureCom->Set_Idx(0);

	return S_OK;
}

_int CUI_MainLogo::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);
	
	if (m_bIncreaseAlpha)
	{
		m_fAlpha += 100.f * fTimeDelta;
		if (m_fAlpha >= 255.f)
		{
			m_bIncreaseAlpha = false;
			m_fAlpha = 255.f;
		}
			
	}
	else
	{
		m_fAlpha -= 100.f * fTimeDelta;
		if (m_fAlpha <= 0.f)
		{
			m_fAlpha = 0.f;
			m_bIncreaseAlpha = true;
		}
			
	}

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_MainLogo::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_MainLogo::Render_Object(void)
{
	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.001f };
	
	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	const LPD3DXFONT& pFont = Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD);

	RECT rc = { WINCX / 2.f - 20.f, WINCY / 2.f + 120.f, WINCX / 2.f + 20.f, WINCY };
	wstring strGuide = L"Press Enter To Start";
	pFont->DrawText(NULL,
		strGuide.c_str(), strGuide.size(), &rc, DT_CENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(int(m_fAlpha), 255, 255, 255));
}

HRESULT CUI_MainLogo::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Banner"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	return S_OK;
}


CUI_MainLogo* CUI_MainLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_MainLogo* pInstance = new CUI_MainLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI_MainLogo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_MainLogo::Free()
{
}
