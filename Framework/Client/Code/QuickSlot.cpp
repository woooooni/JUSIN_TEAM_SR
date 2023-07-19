#include "QuickSlot.h"
#include "Export_Function.h"

CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CQuickSlot::CQuickSlot(const CQuickSlot& rhs)
	: CUI(rhs)
{
}

CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Slot(), E_FAIL);

//	CComponent* pComponent = nullptr;
//
//	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
//	D3DXMatrixIdentity(&m_matView);
//
//	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	pComponent->SetOwner(this);
//	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);
//
//	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_QuickSlot"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	pComponent->SetOwner(this);
//	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
//
//	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	pComponent->SetOwner(this);
//	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
//
//	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
//	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CQuickSlot::Update_Object(const _float& fTimeDelta)
{
	//Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	m_vecSlots[SLOT_ONE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_TWO]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_THREE]->Update_Object(fTimeDelta);
	m_vecSlots[SLOT_FOUR]->Update_Object(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CQuickSlot::LateUpdate_Object(void)
{
	m_vecSlots[SLOT_ONE]->LateUpdate_Object();
	m_vecSlots[SLOT_TWO]->LateUpdate_Object();
	m_vecSlots[SLOT_THREE]->LateUpdate_Object();
	m_vecSlots[SLOT_FOUR]->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	m_vecSlots[SLOT_ONE]->Render_Object();
	m_vecSlots[SLOT_TWO]->Render_Object();
	m_vecSlots[SLOT_THREE]->Render_Object();
	m_vecSlots[SLOT_FOUR]->Render_Object();

	__super::Render_Object();
}

HRESULT CQuickSlot::Add_Slot(void)
{
	m_vecSlots.reserve(SLOTNUM::SLOT_END);

	CUI_SlotOne* p1stSlot = CUI_SlotOne::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(p1stSlot, E_FAIL);
	m_vecSlots.push_back(p1stSlot);

	CUI_SlotTwo* p2ndSlot = CUI_SlotTwo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(p2ndSlot, E_FAIL);
	m_vecSlots.push_back(p2ndSlot);

	CUI_SlotThree* p3rdSlot = CUI_SlotThree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(p3rdSlot, E_FAIL);
	m_vecSlots.push_back(p3rdSlot);

	CUI_SlotFour* p4thSlot = CUI_SlotFour::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(p4thSlot, E_FAIL);
	m_vecSlots.push_back(p4thSlot);

	return S_OK;
}

CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuickSlot* pInstance = new CQuickSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("QuickSlot Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CQuickSlot::Free()
{
}
