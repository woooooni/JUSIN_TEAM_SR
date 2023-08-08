#include "UI_ExclamationMark.h"
#include "Export_Function.h"



CUI_ExclamationMark::CUI_ExclamationMark(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_ExclamationMark::CUI_ExclamationMark(const CUI_ExclamationMark& rhs)
	: CUI(rhs)
{
}

CUI_ExclamationMark::~CUI_ExclamationMark()
{
}

HRESULT CUI_ExclamationMark::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"ExclamationMark", L"Proto_Texture_Quest_ExclamationMark", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"ExclamationMark", TRUE), E_FAIL);

	return S_OK;
}

_int CUI_ExclamationMark::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_ExclamationMark::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_ExclamationMark::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Set_Scale({ 0.3f, 0.8f, 0.f });

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

HRESULT CUI_ExclamationMark::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Quest_ExclamationMark"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

CUI_ExclamationMark* CUI_ExclamationMark::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_ExclamationMark* pInstance = new CUI_ExclamationMark(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("ExclamationMark Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_ExclamationMark::Free()
{
}

