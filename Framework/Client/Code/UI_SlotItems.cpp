#include "UI_SlotItems.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "InventoryMgr.h"

CUI_SlotItems::CUI_SlotItems(LPDIRECT3DDEVICE9 pGraphicDev) 
	: m_eCode(ITEM_CODE::ITEM_END)
	, CUI(pGraphicDev)
{
}

CUI_SlotItems::CUI_SlotItems(const CUI_SlotItems& rhs)
	: CUI(rhs)
	, m_eCode(rhs.m_eCode)
{
}

CUI_SlotItems::~CUI_SlotItems()
{
}

HRESULT CUI_SlotItems::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_pTextureCom->Set_Idx(0);

	m_pAnimator->Add_Animation(L"Base", L"Proto_Texture_Icon_QuickSlot", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);

	return S_OK;
}

_int CUI_SlotItems::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_SlotItems::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_SlotItems::Render_Object(void)
{
	// ¾ÆÀÌÅÛ UI
	_matrix matPreView, matPreProj;
	_vec3	vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
				(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	fRatio = _float(WINCY) / _float(WINCX);
	vScale = _vec3(m_tInfo.fCX * fRatio * 0.5f, m_tInfo.fCY * fRatio * 0.5f, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (m_eCode != ITEM_CODE::ITEM_END)
	{
		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		//		RECT rc = { m_tInfo.fX + 10, m_tInfo.fY + 10, m_tInfo.fX + 40, m_tInfo.fY + 40 };
		//		wstring strCount = to_wstring(CInventoryMgr::GetInstance()->
		//			Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)[_uint(m_eCode)]
		//			->Get_InvenCount());
		//
		//		if (strCount == L"1")
		//			return;
		//
		//		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
		//			->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
		//				DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
		//				D3DCOLOR_ARGB(255, 255, 255, 255));

		//		for (_uint i = 0; i < 4; ++i)
		//		{
		//			RECT rc = { m_tInfo.fX + 10,
		//						m_tInfo.fY + 10,
		//						m_tInfo.fX + 40,
		//						m_tInfo.fY + 40 };
		//
		//			wstring strCount = to_wstring(CInventoryMgr::GetInstance()
		//				->Get_Inventory((CInventoryMgr::INVENTORY_TYPE::CONSUMPSION))[i]->Get_InvenCount());
		//			
		//			if (strCount == L"1")
		//				return;
		//
		//				Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)
		//					->DrawTextW(NULL, strCount.c_str(), strCount.length(), &rc,
		//					DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE,
		//					D3DCOLOR_ARGB(255, 255, 255, 255));
		//		}
		
	}

	// Slot Render
	_vec3 vSlotScale = _vec3(m_tInfo.fCX * fRatio * 0.83f, m_tInfo.fCY * fRatio * 0.83f, 0.f);

	m_pTransformCom->Set_Scale(vSlotScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

}

void CUI_SlotItems::Set_SlotNum(SLOTITEM_NUM eNum)
{
	m_eSlotNum = eNum;

	m_tInfo.fX = 107.f + (_uint)eNum * 78;
	m_tInfo.fY = 115.f;
}

void CUI_SlotItems::Set_ItemCode(const ITEM_CODE& pCOde)
{
	m_eCode = pCOde;
	m_pTextureCom->Set_Idx((_uint)pCOde);
	Set_Filled(m_eCode != ITEM_CODE::ITEM_END);
}

HRESULT CUI_SlotItems::Use_Item()
{
	CInventoryMgr::GetInstance()->Use_Item(m_eCode);
	auto& iter = CInventoryMgr::GetInstance()->Get_Inventory(CInventoryMgr::INVENTORY_TYPE::CONSUMPSION);

	auto item = find_if(iter.begin(), iter.end(), [&](CItem* pItem)->bool
		{
			return pItem->Get_ItemCode() == m_eCode;
		});

	if (item == iter.end())
		Set_ItemCode(ITEM_CODE::ITEM_END);

	return S_OK;
}

HRESULT CUI_SlotItems::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return S_OK;
}

CUI_SlotItems* CUI_SlotItems::Create(LPDIRECT3DDEVICE9 pGraphicDev, SLOTITEM_NUM eNum)
{
	CUI_SlotItems* pInstance = new CUI_SlotItems(pGraphicDev);

	pInstance->Set_SlotNum(eNum);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI SlotItems Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_SlotItems::Free()
{
}


