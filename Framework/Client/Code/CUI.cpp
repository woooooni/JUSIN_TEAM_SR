#include "CUI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_UI)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);
	return S_OK;
}

_int CUI::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUI::Render_Object(void)
{
	CGameObject::Render_Object();
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI* pInstance = new CUI(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CUI::Free()
{
	
}
