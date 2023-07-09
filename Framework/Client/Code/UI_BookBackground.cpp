#include "UI_BookBackground.h"
#include "Export_Function.h"

CUI_BookBackground::CUI_BookBackground(LPDIRECT3DDEVICE9 _pGraphicDev)
    :CUI(_pGraphicDev)
{
}

CUI_BookBackground::CUI_BookBackground(const CUI_BookBackground& rhs)
    : CUI(rhs)
{
}

CUI_BookBackground::~CUI_BookBackground()
{

}

HRESULT CUI_BookBackground::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
    return S_OK;
}

_int CUI_BookBackground::Update_Object(const _float& fTimeDelta)
{
    return S_OK;
}

void CUI_BookBackground::LateUpdate_Object(void)
{
    CUI::LateUpdate_Object();
}

void CUI_BookBackground::Render_Object(void)
{
    CUI::Render_Object();
}

HRESULT CUI_BookBackground::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Page"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

CUI_BookBackground* CUI_BookBackground::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_BookBackground* pInstance = new CUI_BookBackground(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI_BookBackground Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_BookBackground::Free()
{
	__super::Free();
}
