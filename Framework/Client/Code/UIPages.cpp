#include  "../Include/stdafx.h"
#include "UIPages.h"
#include "Export_Function.h"

CUIPages::CUIPages(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUIPages::CUIPages(const CUIPages& rhs)
	: CUI(rhs)
{
}

CUIPages::~CUIPages()
{
}

HRESULT CUIPages::Ready_Object(void)
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

_int CUIPages::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CUIPages::LateUpdate_Object(void)
{
}

void CUIPages::Render_Object(void)
{
	
	// 없어도 되는 부분인데.. 없애면 애니메이션이 이상해짐 ^-^..
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	_vec3 vPos = { ((WINCX) * (1 / m_matProj._11)),
				  ((WINCY) * (1 / m_matProj._22)), 0.f };
	
	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	/////////////////////////////////////


	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();


	/////////////////////////////////////
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	/// /////////////////////////////////

	__super::Render_Object();
}

//bool CUIPages::Move_Frame(float fTimeDelta)
//{
//	m_fFrameCnt += m_fFrameSpeed * fTimeDelta;
//	if (m_fFrameCnt >= m_fEndFrame)
//	{
//		m_fFrameCnt = m_fEndFrame;
//		return true;
//	}
//
//	return false;
//}

CUIPages* CUIPages::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIPages* pInstance = new CUIPages(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUIPages Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIPages::Free()
{
	
}
