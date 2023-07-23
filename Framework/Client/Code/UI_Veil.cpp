#include "UI_Veil.h"
#include "Export_Function.h"

CUI_Veil::CUI_Veil(LPDIRECT3DDEVICE9 _pGraphicDev)
    :CUI(_pGraphicDev)
{
}

CUI_Veil::CUI_Veil(const CUI_Veil& rhs)
    : CUI(rhs)
{
}

CUI_Veil::~CUI_Veil()
{

}

HRESULT CUI_Veil::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = _float(WINCX / 2.f);
	m_tInfo.fY = _float(WINCY / 2.f);

	m_tInfo.fCX = _float(WINCX);
	m_tInfo.fCY = _float(WINCY);

	m_pTransformCom->Set_Scale(_vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f));
		
    return S_OK;
}

_int CUI_Veil::Update_Object(const _float& fTimeDelta)
{
	// Debug_Input();
	Add_RenderGroup(RENDERID::RENDER_UI, this);
	CUI::Update_Object(fTimeDelta);
    return S_OK;
}

void CUI_Veil::LateUpdate_Object(void)
{
    CUI::LateUpdate_Object();
}

void CUI_Veil::Render_Object(void)
{
    CUI::Render_Object();
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
	
	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) *  (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1)  * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUI_Veil::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Veil"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CUI_Veil* CUI_Veil::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Veil* pInstance = new CUI_Veil(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI_Veil Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Veil::Free()
{
	__super::Free();
}
