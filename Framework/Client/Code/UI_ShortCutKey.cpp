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

	return S_OK;
}

_int CUI_ShortCutKey::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	// Key_Input 만들어서 넣기 (대화창 닫히게끔)
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

		_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 0.15f) * (1 / m_matProj._11) ,
			((-2 * (WINCY / 2)) / WINCY + 1.55f) * (1 / m_matProj._22), 0.f };

		m_pTransformCom->Set_Pos(&vPos);

		_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(fWidth * fRatio * 1.1f, fHeight * fRatio * 1.1f, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

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