#include "Icon.h"
#include "Export_Function.h"
#include "GameMgr.h"
CIcon::CIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tIconInfo{}
{
}

CIcon::CIcon(const CIcon& rhs)
	: CUI(rhs), m_tIconInfo(rhs.m_tIconInfo)
{
}

CIcon::~CIcon()
{
}

HRESULT CIcon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CIcon::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CIcon::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CIcon::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_vec3	vPos, vScale;
	_float fWidth, fHeight, fRatio;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
			(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };

	fRatio = _float(WINCY) / _float(WINCX);

	m_pTransformCom->Set_Pos(&vPos);

	switch (m_tIconInfo.eType)
	{
	case ICONTYPE::PLAYERHP_FRAME:
		vScale = _vec3(m_tInfo.fCX * fRatio * 2.5, m_tInfo.fCY * fRatio * 2, 0.f);
		break;

	case ICONTYPE::PLAYERHP_BACK:
		vPos.z = 0.06f;
		m_pTransformCom->Set_Pos(&vPos);
		vScale = _vec3(m_tInfo.fCX * fRatio * 2.5, m_tInfo.fCY * fRatio * 2, 0.f);
		break;

	case ICONTYPE::HEART:
		vScale = _vec3(m_tInfo.fCX * fRatio * 1.6f, m_tInfo.fCY * 1.5 * fRatio, 0.f);
		break;

	case ICONTYPE::QUEST:
		vScale = _vec3(m_tInfo.fCX * fRatio * 1.3, m_tInfo.fCY * fRatio * 1.3, 0.f);
		break;

	case ICONTYPE::KEYBOARD:
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.9, m_tInfo.fCY * fRatio, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_1:
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.4f, m_tInfo.fCY * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_2: // fX 78
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.4f, m_tInfo.fCY * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_3:
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.4f, m_tInfo.fCY * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_4:
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.4f, m_tInfo.fCY * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_L:
		vScale = _vec3(m_tInfo.fCX * fRatio * 0.3f, m_tInfo.fCY * fRatio * 0.3f, 0.f);
		break;

	default:
		break;
	}

	m_pTransformCom->Set_Pos(&vPos);

	CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();;
	_int MaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMaxHp;
	_int HP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iHp;

	RECT rc = { 0, 28, 440, WINCY / 2 };

	string strMaxHP = to_string(MaxHP);
	string strHP = to_string(HP);
	string strBuffer = strHP + " / " + strMaxHP;

	wstring sTemp = wstring(strBuffer.begin(), strBuffer.end());
	LPCWSTR swBuffer = sTemp.c_str();

	Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
		swBuffer, lstrlen(swBuffer), &rc, DT_CENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	__super::Render_Object();
}

void CIcon::Set_Type(ICONTYPE eType)
{
	m_tIconInfo.eType = eType;
}

void CIcon::Key_Input()
{
	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
			m_bShown = true;

		else
			m_bShown = false;
	}
}

HRESULT CIcon::Add_Component(void)
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

	switch (m_tIconInfo.eType)
	{
	case ICONTYPE::PLAYERHP_FRAME:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 420.f;
		m_tInfo.fY = WINCY / 2 - 390.f;
		break;

	case ICONTYPE::PLAYERHP_BACK:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 420.f;
		m_tInfo.fY = WINCY / 2 - 390.f;
		break;

	case ICONTYPE::HEART:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Heart"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 605.f;
		m_tInfo.fY = WINCY / 2 - 390.f;
		break;

	case ICONTYPE::KEYBOARD:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Keyboard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 605.f;
		m_tInfo.fY = WINCY / 2 - 320.f;
		break;

	case ICONTYPE::QUEST:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Quest"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 234.f;
		m_tInfo.fY = WINCY / 2 - 390.f;
		break;

	case ICONTYPE::KEYBUTTON_L:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_L"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 220.f;
		m_tInfo.fY = WINCY / 2 - 375.f;
		break;

	case ICONTYPE::KEYBUTTON_1:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_1"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 510.f;
		m_tInfo.fY = WINCY / 2 - 340.f;
		break;

	case ICONTYPE::KEYBUTTON_2:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_2"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 432.f;
		m_tInfo.fY = WINCY / 2 - 340.f;
		break;

	case ICONTYPE::KEYBUTTON_3:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_3"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 354.f;
		m_tInfo.fY = WINCY / 2 - 340.f;
		break;

	case ICONTYPE::KEYBUTTON_4:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_4"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = WINCX / 2 - 276.f;
		m_tInfo.fY = WINCY / 2 - 340.f;
		break;

	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}

CIcon* CIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, ICONTYPE eType)
{
	CIcon* pInstance = new CIcon(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Icon Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CIcon::Free()
{
}
