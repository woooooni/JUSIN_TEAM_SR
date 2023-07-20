#include "../Header/UI_LoadingBackGround.h"
#include "Export_Function.h"

CUI_LoadingBackGround::CUI_LoadingBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_LoadingBackGround::CUI_LoadingBackGround(const CUI_LoadingBackGround& rhs)
	: CUI(rhs)
{
}

CUI_LoadingBackGround::~CUI_LoadingBackGround()
{
}

HRESULT CUI_LoadingBackGround::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = _float(WINCX / 2.f);
	m_tInfo.fY = _float(WINCY / 2.f);

	m_tInfo.fCX = _float(WINCX);
	m_tInfo.fCY = _float(WINCY);

	m_pTransformCom->Set_Scale(_vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f));
	m_pTextureCom->Set_Idx(0);

	return S_OK;
}

_int CUI_LoadingBackGround::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);
	// Debug_Input();

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_LoadingBackGround::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_LoadingBackGround::Render_Object(void)
{
	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.001f };
	
	m_pTransformCom->Set_Pos(&vPos);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	RECT rc = { m_tInfo.fX, m_tInfo.fY + 40.f, m_tInfo.fX, WINCY };

	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
		L"로딩 중..", lstrlen(L"로딩 중.."), &rc, DT_CENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

HRESULT CUI_LoadingBackGround::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_LoadingBackGround"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	return S_OK;
}


CUI_LoadingBackGround* CUI_LoadingBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_LoadingBackGround* pInstance = new CUI_LoadingBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Shop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_LoadingBackGround::Free()
{
}
