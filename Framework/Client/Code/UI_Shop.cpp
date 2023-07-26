#include "UI_Shop.h"
#include "Export_Function.h"

CUI_Shop::CUI_Shop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_Shop::CUI_Shop(const CUI_Shop& rhs)
	: CUI(rhs)
{
}

CUI_Shop::~CUI_Shop()
{
}

HRESULT CUI_Shop::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = WINCX / 2 - 180.f;
	m_tInfo.fY = WINCY / 2 + 80.f;

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_vecShopIcon.resize(SHOPITEMTYPE::SHOPITEM_END);

	for (_uint i = (_uint)SHOPITEMTYPE::SHOPKEY_L; i < (_uint)SHOPITEMTYPE::SHOPITEM_END; i++)
	{
		m_vecShopIcon[i] = CUI_ItemInfo::Create(m_pGraphicDev, (SHOPITEMTYPE)i);
	}


	m_pCursor = CUI_Cursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCursor, E_FAIL);

	return S_OK;
}

_int CUI_Shop::Update_Object(const _float& fTimeDelta)
{
	Key_Input();
	if (m_bShown)
	{
		m_pCursor->Update_Object(fTimeDelta);

		for (auto& iter : m_vecShopIcon)
			iter->Update_Object(fTimeDelta);

		__super::Update_Object(fTimeDelta);
	}

	return S_OK;
}

void CUI_Shop::LateUpdate_Object(void)
{
	m_pCursor->LateUpdate_Object();

	for (auto& iter : m_vecShopIcon)
		iter->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CUI_Shop::Render_Object(void)
{
	_matrix matPreView, matPreProj;

	_vec3 vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.001f };
	m_pTransformCom->Set_Pos(&vPos);

	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(m_tInfo.fCX * fRatio * 2.f, m_tInfo.fCY * fRatio * 2.f, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

HRESULT CUI_Shop::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Background"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CUI_Shop::Key_Input()
{
	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
		{
			m_bShown = true;
			m_pCursor->Set_Shown(true);

			for (auto& iter : m_vecShopIcon)
				dynamic_cast<CUI_ItemInfo*>(iter)->Set_Shown(true);
		}

		else
		{
			m_bShown = false;
			m_pCursor->Set_Shown(false);

			for (auto& iter : m_vecShopIcon)
				dynamic_cast<CUI_ItemInfo*>(iter)->Set_Shown(false);
		}
	}

}

void CUI_Shop::Set_Item()
{

}

CUI_Shop* CUI_Shop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Shop* pInstance = new CUI_Shop(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Shop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Shop::Free()
{
}
