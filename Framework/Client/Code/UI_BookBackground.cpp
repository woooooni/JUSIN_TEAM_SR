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

	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = WINCY / 2.f;

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

    return S_OK;
}

_int CUI_BookBackground::Update_Object(const _float& fTimeDelta)
{
	// Debug_Input();
	Add_RenderGroup(RENDERID::RENDER_UI, this);
	CUI::Update_Object(fTimeDelta);
    return S_OK;
}

void CUI_BookBackground::LateUpdate_Object(void)
{
    CUI::LateUpdate_Object();
}

void CUI_BookBackground::Render_Object(void)
{
    CUI::Render_Object();
		_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
	
	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) *  (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1)  * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_AdventureBook_Background"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

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
