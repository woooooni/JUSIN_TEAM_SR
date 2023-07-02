#include "Export_Function.h"
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

	m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);

	m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);

	m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

	return S_OK;
}


_int CUI::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
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
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
	
	_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) *  (1 / m_matProj._11) , ((-2 * (WINCY / 2)) / WINCY + 1)  * (1 / m_matProj._22), 0.f };
	
	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = fHeight / fWidth;
	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW,	&m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
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
	CGameObject::Free();
}
