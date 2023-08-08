#include "../Header/UI_LoadingOgu.h"
#include "Export_Function.h"

CUI_LoadingOgu::CUI_LoadingOgu(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_LoadingOgu::CUI_LoadingOgu(const CUI_LoadingOgu& rhs)
	: CUI(rhs)
{
}

CUI_LoadingOgu::~CUI_LoadingOgu()
{
}

HRESULT CUI_LoadingOgu::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = _float(WINCX / 2.f);
	m_tInfo.fY = _float(WINCY / 2.f) - 20.f;

	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 100.f;

	m_pTransformCom->Set_Scale(_vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f));

	m_pAnimator->Add_Animation(L"Ogu_Loading", L"Proto_Texture_OguLoading", 0.1f);
	m_pAnimator->Play_Animation(L"Ogu_Loading", true);

	return S_OK;
}

_int CUI_LoadingOgu::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);
	Debug_Input();

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_LoadingOgu::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_LoadingOgu::Render_Object(void)
{
	_matrix matPreView, matPreProj;


	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
	
	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}

HRESULT CUI_LoadingOgu::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);


	return S_OK;
}


CUI_LoadingOgu* CUI_LoadingOgu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_LoadingOgu* pInstance = new CUI_LoadingOgu(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Shop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_LoadingOgu::Free()
{
}
