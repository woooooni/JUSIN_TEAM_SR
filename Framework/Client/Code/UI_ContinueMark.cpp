#include "UI_ContinueMark.h"
#include "Export_Function.h"



CUI_ContinueMark::CUI_ContinueMark(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_ContinueMark::CUI_ContinueMark(const CUI_ContinueMark& rhs)
	: CUI(rhs)
{
}

CUI_ContinueMark::~CUI_ContinueMark()
{
}

HRESULT CUI_ContinueMark::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	return S_OK;
}

_int CUI_ContinueMark::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_ContinueMark::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_ContinueMark::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Set_Scale({ 1.f, 1.f, 0.f });

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

HRESULT CUI_ContinueMark::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Quest_ContinueMark"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CUI_ContinueMark* CUI_ContinueMark::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_ContinueMark* pInstance = new CUI_ContinueMark(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("ContinueMark Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_ContinueMark::Free()
{
}

