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
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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

	return S_OK;
}

_int CQuickSlot::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	// Key_Input?

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CQuickSlot::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CQuickSlot::Render_Object(void)
{
	// for문으로 4개 Render
	//for (int i = 0; i < 4; i++)
	//{
	//	_float fX = 1.86f; // 간격 조절을 위한 X 기본값
	//	_float fPos = _float(i * 0.03);

		_matrix matPreView, matPreProj;
		_vec3 vPos;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	//	_vec3 vPos = { (WINCX / WINCX - (fX + i)) * (1 / m_matProj._11) ,
	//			 ((-1 * WINCY) / WINCY + 1.8f) * (1 / m_matProj._22), 0.f };

		switch (m_tInfo.eType)
		{
		case SLOTNUM::SLOT_ONE:
			vPos = { (WINCX / WINCX - 1.84f) * (1 / m_matProj._11) ,
					((-1 * WINCY) / WINCY + 1.77f) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_TWO:
			vPos = { (WINCX / WINCX - 1.72f) * (1 / m_matProj._11) ,
					((-1 * WINCY) / WINCY + 1.77f) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_THREE:
			vPos = { (WINCX / WINCX - 1.6f) * (1 / m_matProj._11) ,
					((-1 * WINCY) / WINCY + 1.77f) * (1 / m_matProj._22), 0.f };
			break;

		case SLOTNUM::SLOT_FOUR:
			vPos = { (WINCX / WINCX - 1.48f) * (1 / m_matProj._11) ,
					((-1 * WINCY) / WINCY + 1.77f) * (1 / m_matProj._22), 0.f };
			break;

		default:
			break;
		}

		m_pTransformCom->Set_Pos(&vPos);

		_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		_float fRatio = _float(WINCY) / _float(WINCX);

		_vec3 vScale = _vec3(fWidth * fRatio * 0.85, fHeight * 0.85 * fRatio, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	//}

	__super::Render_Object();
}

HRESULT CQuickSlot::Add_Component(void)
{
	return S_OK;
}

void CQuickSlot::Set_Type(SLOTNUM eType)
{
	m_tInfo.eType = eType;
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
