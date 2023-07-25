#include "UI_HPBar.h"
#include "Export_Function.h"

CUI_HPBar::CUI_HPBar(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_HPBar::CUI_HPBar(const CUI_HPBar& rhs) : CUI(rhs)
{
}

CUI_HPBar::~CUI_HPBar()
{
}

HRESULT CUI_HPBar::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_HPGauge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_tInfo.fX = WINCX / 2 - 420.f;
	m_tInfo.fY = WINCY / 2 - 390.f;

	return S_OK;
}

_int CUI_HPBar::Update_Object(const _float& fTimeDelta)
{
	CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	m_iMaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMaxHp;
	m_iHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iHp;

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_HPBar::LateUpdate_Object(void)
{
	_float fMaxHP = _float(m_iMaxHP);
	_float fCurHP = _float(m_iHP);
	_float fHP = fCurHP / fMaxHP;

	_float fIndex = m_tInfo.fCX - m_tInfo.fCX * fHP;

	m_tInfo.fX = (WINCX / 2 - 420.f) - fIndex + (fIndex * 0.15f);

	__super::LateUpdate_Object();
}

void CUI_HPBar::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_float fCurHP, fMaxHP, fHP;
	
	fMaxHP = _float(m_iMaxHP);
	fCurHP = _float(m_iHP);
	fHP = fCurHP / fMaxHP;
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	// 프레임 맞춰서 Pos 1차 수정필요. Scale.x 줄어든만큼 위치값옮기기
	_float fOriginWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fWidth = fOriginWidth * fHP; // HPBar 남은 길이
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	_float fRatio = _float(WINCY) / _float(WINCX);

	if (fWidth == 0)
		fWidth = 0.005f;

	// HPBar 줄어든길이 -> 줄어든 길이만큼 Pos를 옮겨줘야함.
	_float fX = fOriginWidth - fWidth; // HPBar 줄어든 길이

	// HP Bar 프레임 키운만큼 같이 확대시킴
	_vec3 vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);
	m_pTransformCom->Set_Scale(vScale);

	_vec3 vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.05f };

	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	__super::Render_Object();
}

CUI_HPBar* CUI_HPBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_HPBar* pInstance = new CUI_HPBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("HP Bar Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_HPBar::Free()
{
}
