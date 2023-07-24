#include "UI_SlotItems.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "InventoryMgr.h"

CUI_SlotItems::CUI_SlotItems(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_SlotItems::CUI_SlotItems(const CUI_SlotItems& rhs)
	: CUI(rhs)
{
}

CUI_SlotItems::~CUI_SlotItems()
{
}

HRESULT CUI_SlotItems::Ready_Object(void)
{
	CComponent* pComponent = nullptr;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_pTextureCom->Set_Idx(0);

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
	// 아이템 UI
	_matrix matPreView, matPreProj;
	_vec3	vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
				(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };
	m_pTransformCom->Set_Pos(&vPos);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	fRatio = _float(WINCY) / _float(WINCX);
	vScale = _vec3(m_tInfo.fCX * fRatio * 0.5f, m_tInfo.fCY * fRatio * 0.5f, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	// 아이템 개수
//	for (_uint i = 0;
//		i < CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx).size(); ++i)
//	{
//		auto iter = Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD);
//		RECT rc = { (i % 5) * 115.f + 310.f + 30.f , (i / 5) * 115.f + 310.f + 30.f, (i % 5) * 115.f + 310.f + 40.f , (i / 5) * 115.f + 310.f + 40.f };
//		wstring tmp = to_wstring(CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[i]->Get_InvenCount());
//
//		if (tmp == L"1")
//			continue;
//
//		iter->DrawTextW(NULL, tmp.c_str(), tmp.length(), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
//	}
}

void CUI_SlotItems::Set_SlotNum(SLOTITEM_NUM eNum)
{
	m_eSlotNum = eNum;

}

HRESULT CUI_SlotItems::Add_Component(void)
{
	CQuickSlot* pSlot = CUIMgr::GetInstance()->Get_Slots();
	
	switch (m_eSlotNum)
	{
	case SLOTITEM_NUM::FIRST:
		m_tInfo.fX = pSlot->Get_SlotOne()->Get_UI_Info().fX;
		m_tInfo.fY = pSlot->Get_SlotOne()->Get_UI_Info().fY;
		break;

	case SLOTITEM_NUM::SECOND:
		m_tInfo.fX = pSlot->Get_SlotTwo()->Get_UI_Info().fX;
		m_tInfo.fY = pSlot->Get_SlotTwo()->Get_UI_Info().fY;
		break;

	case SLOTITEM_NUM::THIRD:
		m_tInfo.fX = pSlot->Get_SlotThree()->Get_UI_Info().fX;
		m_tInfo.fY = pSlot->Get_SlotThree()->Get_UI_Info().fY;
		break;

	case SLOTITEM_NUM::FOURTH:
		m_tInfo.fX = pSlot->Get_SlotFour()->Get_UI_Info().fX;
		m_tInfo.fY = pSlot->Get_SlotFour()->Get_UI_Info().fY;
		break;

	default:
		break;
	}

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


