#include "UI_Wallet.h"
#include "Export_Function.h"
#include "GameMgr.h"

CUI_Wallet::CUI_Wallet(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_Wallet::CUI_Wallet(const CUI_Wallet& rhs) : CUI(rhs)
{
}

CUI_Wallet::~CUI_Wallet()
{
}

HRESULT CUI_Wallet::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_PlayerWallet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_tInfo.fX = WINCX / 2 + 545.f;
	m_tInfo.fY = WINCY / 2 - 390.f;

	m_bShown = true;
	Set_Active(true);

	return S_OK;
}

_int CUI_Wallet::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
	{
		m_fShowingTime = 0.f;
	}

	if (Is_Active())
	{
		m_bShown = true;

		if (m_fShowingTime > 20.f)
			if (m_iAlpha <= 255)
				m_iAlpha -= fTimeDelta * 100;
	}

	m_fShowingTime += 10 * fTimeDelta;

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_Wallet::LateUpdate_Object(void)
{
	if (Is_Active())
	{
		if (m_iAlpha <= 0)
		{
			m_bShown = false;
			Set_Active(false);
			//m_iAlpha = 255;
		}
	}

	__super::LateUpdate_Object();
}

void CUI_Wallet::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3 vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	fRatio = _float(WINCY) / _float(WINCX);

	vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
			(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio, 0.f);
	m_pTransformCom->Set_Scale(vScale);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();;

	if (pPlayer != nullptr)
	{
		m_iPlayerMoney = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMoney;
	}

	RECT rc = { 1110, 25, WINCX, WINCY / 2 };

	string strMoney = to_string(m_iPlayerMoney);
	wstring sTemp = wstring(strMoney.begin(), strMoney.end());
	LPCWSTR swBuffer = sTemp.c_str();

	if (m_bShown)
	{
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			swBuffer, lstrlen(swBuffer), &rc, DT_CENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	__super::Render_Object();
}

CUI_Wallet* CUI_Wallet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Wallet* pInstance = new CUI_Wallet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Wallet Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Wallet::Free()
{
}
