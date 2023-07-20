#include "InventoryUI.h"
#include	"Export_Function.h"
#include		"InvenTabButton.h"
#include	"InventoryMgr.h"

CInventoryUI::CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev), m_iCurIdx(0)
{
}

CInventoryUI::CInventoryUI(const CInventoryUI& rhs) : CUI(rhs), m_iCurIdx(rhs.m_iCurIdx)
{
}



CInventoryUI::~CInventoryUI()
{
}

HRESULT CInventoryUI::Ready_Object(void)
{
	FAILED_CHECK(__super::Ready_Object());

	CComponent* pCom = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Frame"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE, pCom });


	return S_OK;
}

_int CInventoryUI::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_UI, this);
	Debug_Input();
	for (auto& iter : m_vecTabButton)
	{
		iter->Update_Object(fTimeDelta);
	}

	return __super::Update_Object(fTimeDelta);
}

void CInventoryUI::LateUpdate_Object(void)
{
	for (auto& iter : m_vecTabButton)
	{
		iter->LateUpdate_Object();
	}
	__super::LateUpdate_Object();
}

void CInventoryUI::Render_Object(void)
{
	Ready_TransWorld();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
	for (auto& iter : m_vecTabButton)
	{
		iter->Render_Object();
	}

	for (_uint i = 0; i < CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurIdx).size(); ++i)
	{
		CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurIdx)[i]->Get_TransformCom()->Set_Pos(&Trans_WinPos({ (_float)((i % 5) * 100), (_float)((i / 5 + 2) * 100), 0 }));
		CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurIdx)[i]->Render_UI();
	}
}

CInventoryUI* CInventoryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventoryUI* ret = new CInventoryUI(pGraphicDev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("InvenUI Create Failed");
		return nullptr;
	}

	ret->m_tInfo.fX = WINCX * 0.5f;
	ret->m_tInfo.fY = WINCY * 0.5f;
	ret->m_tInfo.fCX = 624.f;
	ret->m_tInfo.fCY = 400.f;


	for (_uint i = (_uint)ITEM_TYPE::EQUIPMENT; i < (_uint)ITEM_TYPE::COIN; i++)
	{
		CInvenTabButton* inven = CInvenTabButton::Create(pGraphicDev, ret, { i * 100.f, 100.f, 1.f });
		inven->Get_TextureCom()->Set_Idx(i);
		ret->m_vecTabButton.push_back(inven);

	}

	return ret;
}

void CInventoryUI::Free()
{
	for (auto& iter : m_vecTabButton)
	{
		iter->Free();
	}
	m_vecTabButton.clear();
	__super::Free();
}
