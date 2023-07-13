#include "UI_Totem.h"
#include "Export_Function.h"

CUI_Totem::CUI_Totem(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_Totem::CUI_Totem(const CUI_Totem& rhs) : CUI(rhs)
{
}

CUI_Totem::~CUI_Totem()
{
}

HRESULT CUI_Totem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
	
	// 좌측 현재 PlayerLife 출력 Render
	if (m_eTotemType == TOTEMTYPE::CURR)
	{
		switch (m_iCurrValue)
		{
		case 1:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem1"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 2:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem2"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 3:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem3"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 4:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem4"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 5:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem5"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 6:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem6"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		default:
			break;
		}
	}

	// 우측 Maximum 출력 Render
	if (m_eTotemType == TOTEMTYPE::MAX)
	{
		switch (m_iMaximum)
		{
		case 3:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem3"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 4:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem4"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 5:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem5"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		case 6:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Totem6"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			pComponent->SetOwner(this);
			m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
			break;

		default: // 기본 3이 출력되게할까함
			break;
		}
	}

	return S_OK;
}

void CUI_Totem::Set_Type(TOTEMTYPE eType)
{
	m_eTotemType = eType;
}

HRESULT CUI_Totem::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CUI_Totem::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	//m_iMaximum = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMaxHp;
	//m_iCurrValue = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iHp;

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_Totem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_Totem::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3 vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	// 중간 슬래시는 -1.67f
	if (m_eTotemType == TOTEMTYPE::CURR)
	{
		// Pos, Scale 수정 필요
		vPos = { (WINCX / WINCX - 1.72f) * (1 / m_matProj._11), // -1.75
				 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.75f, fHeight * fRatio * 0.75f, 0.f);
	}

	if (m_eTotemType == TOTEMTYPE::MAX)
	{
		// Pos, Scale 수정 필요
		vPos = { (WINCX / WINCX - 1.62f) * (1 / m_matProj._11), // -1.59
				 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.75f, fHeight * fRatio * 0.75f, 0.f);
	}

	m_pTransformCom->Set_Pos(&vPos);

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

CUI_Totem* CUI_Totem::Create(LPDIRECT3DDEVICE9 pGraphicDev, TOTEMTYPE eType)
{
	CUI_Totem* pInstance = new CUI_Totem(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Totem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Totem::Free()
{
}
