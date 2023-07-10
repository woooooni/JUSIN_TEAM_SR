#include "UI_NameTag.h"
#include "Export_Function.h"
#include "../Include/stdafx.h"

CUI_NameTag::CUI_NameTag(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_NameTag::CUI_NameTag(const CUI_NameTag& rhs)
	: CUI(rhs)
{
}

CUI_NameTag::~CUI_NameTag()
{
}

HRESULT CUI_NameTag::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_NameTag"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return E_NOTIMPL;
}

_int CUI_NameTag::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CUI_NameTag::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_NameTag::Render_Object(void)
{
	_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (WINCY / 2)) / WINCY + 0.5f) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(fWidth * fRatio * 0.8, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CUI_NameTag* CUI_NameTag::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NameTag* pInstance = new CUI_NameTag(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC NameTag Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NameTag::Free()
{
}
