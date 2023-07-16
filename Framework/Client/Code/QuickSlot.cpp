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
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_QuickSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fX = 0.f;
	m_tInfo.fY = 0.f;
	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CQuickSlot::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CQuickSlot::LateUpdate_Object(void)
{
	switch (m_tSlotInfo.eType)
	{
	case SLOTNUM::SLOT_ONE:
		m_tInfo.fX = -536.f;
		m_tInfo.fY = -320.f;
		break;

	case SLOTNUM::SLOT_TWO:
		m_tInfo.fX = -460.5f;
		m_tInfo.fY = -320.f;
		break;

	case SLOTNUM::SLOT_THREE:
		m_tInfo.fX = -385.f;
		m_tInfo.fY = -320.f;
		break;

	case SLOTNUM::SLOT_FOUR:
		m_tInfo.fX = -309.5f;
		m_tInfo.fY = -320.f;
		break;

	default:
		break;
	}
	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
		_matrix matPreView, matPreProj;
		_vec3 vPos;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		switch (m_tSlotInfo.eType)
		{
		case SLOTNUM::SLOT_ONE:
			vPos = { (2 * (m_tInfo.fX) / WINCX) * (1 / m_matProj._11) ,
		(-2 * (m_tInfo.fY) / WINCY) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_TWO:
			vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_THREE:
			vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_FOUR:
			vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };
			break;

		default:
			break;
		}

		m_pTransformCom->Set_Pos(&vPos);

		_float fRatio = _float(WINCY) / _float(WINCX);

		_vec3 vScale = _vec3(m_tInfo.fCX * fRatio * 0.85, m_tInfo.fCY * 0.85 * fRatio, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	__super::Render_Object();
}

HRESULT CQuickSlot::Add_Component(void)
{
	return S_OK;
}

void CQuickSlot::Set_Type(SLOTNUM eType)
{
	m_tSlotInfo.eType = eType;
}

CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, SLOTNUM eType)
{
	CQuickSlot* pInstance = new CQuickSlot(pGraphicDev);

	pInstance->Set_Type(eType);

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
