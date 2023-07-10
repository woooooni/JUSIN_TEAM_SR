#include  "../Include/stdafx.h"
#include "UI_Pages.h"
#include "Export_Function.h"

CUI_Pages::CUI_Pages(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_Pages::CUI_Pages(const CUI_Pages& rhs)
	: CUI(rhs)
{
}

CUI_Pages::~CUI_Pages()
{
}

HRESULT CUI_Pages::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"UI_Pages", L"Proto_Texture_UI_Page", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"UI_Pages", TRUE), E_FAIL);

	return S_OK;
}

_int CUI_Pages::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CUI_Pages::LateUpdate_Object(void)
{
}

void CUI_Pages::Render_Object(void)
{
	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	// WINCX / 2 -> pos.x
	// WINCY / 2 -> pos.y

	_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (WINCY / 2)) / WINCY + 0.5f) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}


CUI_Pages* CUI_Pages::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Pages* pInstance = new CUI_Pages(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI_Pages Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Pages::Free()
{
	__super::Free();
}
