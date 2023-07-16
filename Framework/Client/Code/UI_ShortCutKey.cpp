#include "UI_ShortCutKey.h"
#include "Export_Function.h"

CUI_ShortCutKey::CUI_ShortCutKey(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_ShortCutKey::CUI_ShortCutKey(const CUI_ShortCutKey& rhs)
	: CUI(rhs)
{
}

CUI_ShortCutKey::~CUI_ShortCutKey()
{
}

HRESULT CUI_ShortCutKey::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_ShortKey"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fX = 540.f;
	m_tInfo.fY = -240.f;

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CUI_ShortCutKey::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_ShortCutKey::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_ShortCutKey::Render_Object(void)
{
	if (m_bShown)
	{
		_matrix matPreView, matPreProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		_vec3 vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
						(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };

		m_pTransformCom->Set_Pos(&vPos);

		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(m_tInfo.fCX * fRatio * 1.1f, m_tInfo.fCY * fRatio * 1.1f, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	}
}

CUI_ShortCutKey* CUI_ShortCutKey::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_ShortCutKey* pInstance = new CUI_ShortCutKey(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI ShortCutKey Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_ShortCutKey::Free()
{
}