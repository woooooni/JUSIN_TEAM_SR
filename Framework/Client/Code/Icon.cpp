#include "Icon.h"
#include "Export_Function.h"

CIcon::CIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tInfo{}
{
}

CIcon::CIcon(const CIcon& rhs)
	: CUI(rhs), m_tInfo(rhs.m_tInfo)
{
}

CIcon::~CIcon()
{
}

HRESULT CIcon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CIcon::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

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

	switch (m_tInfo.eType)
	{
		// 메인 컴포넌트 첫번째 줄
	case ICONTYPE::PLAYERHP_FRAME:
		vPos = { (WINCX / WINCX - 1.66f) * (1 / m_matProj._11) ,
		 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);
		break;

	case ICONTYPE::PLAYERHP_BACK:
		vPos = { (WINCX / WINCX - 1.66f) * (1 / m_matProj._11) ,
				((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 2.5, fHeight * fRatio * 2, 0.f);
		break;

	case ICONTYPE::HEART:
		vPos = { (WINCX / WINCX - 1.95f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f }; // y좌표 -0.2
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 1.6f, fHeight * 1.5 * fRatio, 0.f);
		break;

	case ICONTYPE::QUEST:
		vPos = { (WINCX / WINCX - 1.37f) * (1 / m_matProj._11) , // 1.485
				 ((-1 * WINCY) / WINCY + 1.91f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 1.3, fHeight * fRatio * 1.3, 0.f);
		break;

		// 메인 컴포넌트 두번째 줄
	case ICONTYPE::KEYBOARD:
		vPos = { (WINCX / WINCX - 1.95f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.75f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.9, fHeight * fRatio, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_1:
		vPos = { (WINCX / WINCX - 1.8f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.8f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.4f, fHeight * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_2:
		vPos = { (WINCX / WINCX - 1.68f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.8f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.4f, fHeight * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_3:
		vPos = { (WINCX / WINCX - 1.56f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.8f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.4f, fHeight * fRatio * 0.4f, 0.f);
		break;

	case ICONTYPE::KEYBUTTON_4:
		vPos = { (WINCX / WINCX - 1.44f) * (1 / m_matProj._11) ,
				 ((-1 * WINCY) / WINCY + 1.8f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.4f, fHeight * fRatio * 0.4f, 0.f);
		break;

		// ETC.
	case ICONTYPE::KEYBUTTON_L:
		vPos = { (WINCX / WINCX - 1.34f) * (1 / m_matProj._11) , // - 1.353f
				 ((-1 * WINCY) / WINCY + 1.88f) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(fWidth * fRatio * 0.3f, fHeight * fRatio * 0.3f, 0.f);
		break;

	default:
		break;
	}

	m_pTransformCom->Set_Pos(&vPos);

	// Player HP TextOut //

	_int MaxHP = 3;
	_int HP = 1;

	// 현재 MaxHP, HP 모두 0으로 되어있어 주석처리함. Player 세팅 완료시 사용
	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	//MaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iMaxHp;
	//HP = dynamic_cast<CPlayer*>(pPlayer)->Get_PlayerStat().iHp;

	RECT rc = { 0, 25, 440, WINCY / 2 };
	//TCHAR szBuffer[32] = L"1 / 3";

	string strMaxHP = to_string(MaxHP);
	string strHP = to_string(HP);
	string strBuffer = strHP + " / " + strMaxHP;

	wstring sTemp = wstring(strBuffer.begin(), strBuffer.end());
	LPCWSTR swBuffer = sTemp.c_str();

	CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
		swBuffer, lstrlen(swBuffer), &rc, DT_CENTER | DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255));

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

void CIcon::Set_Type(ICONTYPE eType)
{
	m_tInfo.eType = eType;
}

void CIcon::Key_Input()
{
	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
			m_bShown = TRUE;

		else
			m_bShown = FALSE;
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

	switch (m_tInfo.eType)
	{
	case ICONTYPE::PLAYERHP_FRAME:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::PLAYERHP_BACK:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::HEART:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Heart"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBOARD:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Keyboard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::QUEST:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Quest"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBUTTON_L:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_L"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBUTTON_1:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_1"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBUTTON_2:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_2"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBUTTON_3:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_3"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case ICONTYPE::KEYBUTTON_4:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_KeyButton_4"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
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
