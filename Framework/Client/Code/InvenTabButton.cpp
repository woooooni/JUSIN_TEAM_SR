#include "InvenTabButton.h"
#include	"Export_Function.h"

CInvenTabButton::CInvenTabButton(LPDIRECT3DDEVICE9 pG) : CUI(pG)
{
}

CInvenTabButton::CInvenTabButton(const CInvenTabButton& rhs) : CUI(rhs)
{
}

CInvenTabButton::~CInvenTabButton()
{
}

HRESULT CInvenTabButton::Ready_Object(void)
{
	CComponent* pC = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_TabIcon"));
	NULL_CHECK_RETURN(pC, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE , pC });

	return __super::Ready_Object();
}

_int CInvenTabButton::Update_Object(const _float& fTimeDelta)
{
	return __super::Update_Object(fTimeDelta);
}

void CInvenTabButton::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CInvenTabButton::Render_Object(void)
{
	__super::Render_Object();
}

CInvenTabButton* CInvenTabButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CInventoryUI* pOwner, const _vec3& pPos)
{
	CInvenTabButton* ret = new CInvenTabButton(pGraphicDev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("InvenBut Create Failed");
		return nullptr;
	}

	ret->m_tInfo.fX = pPos.x;
	ret->m_tInfo.fY = pPos.y;
	ret->m_tInfo.fCX = 100.f;
	ret->m_tInfo.fCY = 100.f;
	return ret;
}


void CInvenTabButton::Free()
{
}
