#include "UI_Notification.h"
#include "Export_Function.h"

CUI_Notification::CUI_Notification(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_Notification::CUI_Notification(const CUI_Notification& rhs) : CUI(rhs)
{
}

CUI_Notification::~CUI_Notification()
{
}

HRESULT CUI_Notification::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = WINCY / 2.f;

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	m_fCurWidth = 64.f;
	m_fCurHeight = 43.f;

	return S_OK;
}

_int CUI_Notification::Update_Object(const _float& fTimeDelta)
{
//	if (!Is_Active())
//	{
//		m_fCurWidth = 64.f;
//		m_fCurHeight = 43.f;
//	}

	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_Notification::LateUpdate_Object(void)
{
	float fTimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	_float fRatio = _float(WINCY) / _float(WINCX);

	m_fMaxWidth = fWidth * fRatio * 2.1f * m_fWidthRatio;
	m_fMaxHeight = fHeight * fRatio * 2.1f * m_fHeightRatio;

	if (m_fCurWidth < m_fMaxWidth)
		m_fCurWidth += m_fCurWidth * fRatio * fTimeDelta * m_fSpeed;

	if (m_fCurHeight < m_fMaxHeight)
		m_fCurHeight += m_fCurHeight * fRatio * fTimeDelta * m_fSpeed;

	__super::LateUpdate_Object();
}

void CUI_Notification::Render_Object(void)
{
	CUI::Render_Object();


	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) ,
				((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);

	_vec3 vScale = _vec3(m_fCurWidth, m_fCurHeight, 0.f);
	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUI_Notification::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_NoticeFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CUI_Notification::Set_Ratio(_float fWidthRatio, _float fHeightRatio)
{
	m_fWidthRatio = fWidthRatio;
	m_fHeightRatio = fHeightRatio;
}

void CUI_Notification::Set_InitSize(_float _fCurWidth, _float _fCurHeight)
{
	m_fCurWidth = _fCurWidth;
	m_fCurHeight = _fCurHeight;
}

CUI_Notification* CUI_Notification::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fWidthRatio, _float fHeightRatio)
{
	CUI_Notification* pInstance = new CUI_Notification(pGraphicDev);

	pInstance->Set_Ratio(fWidthRatio, fHeightRatio);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Notification Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Notification::Free()
{
}
