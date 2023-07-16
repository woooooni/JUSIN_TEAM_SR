#include "UI_BossHP.h"
#include "Export_Function.h"

CUI_BossHP::CUI_BossHP(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_BossHP::CUI_BossHP(const CUI_BossHP& rhs) : CUI(rhs)
{
}

CUI_BossHP::~CUI_BossHP()
{
}

HRESULT CUI_BossHP::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = WINCY / 2.f - 350.f;

	m_vDefaultPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11),
					((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	return S_OK;
}

_int CUI_BossHP::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	// Test : SunGollem
	CGameObject* pBossMonster = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER)->Find_GameObject(L"SunGollem");

	if (pBossMonster != nullptr)
	{
		m_iMaxHP = dynamic_cast<CSunGollem*>(pBossMonster)->Get_Stat().iMaxHp;
		m_iCurHP = dynamic_cast<CSunGollem*>(pBossMonster)->Get_Stat().iHp;
	}

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_BossHP::LateUpdate_Object(void)
{
	_float fMaxHP = _float(m_iMaxHP);
	_float fCurHP = _float(m_iCurHP);
	_float fHP = fCurHP / fMaxHP;

	_float fIndex = m_tInfo.fCX - m_tInfo.fCX * fHP;

	m_tInfo.fX = WINCX / 2.f - fIndex * 1.4f;

	__super::LateUpdate_Object();
}

void CUI_BossHP::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	_float fCurHP, fMaxHP, fHP;

	if (m_bShown)
	{
		fMaxHP = _float(m_iMaxHP);
		fCurHP = _float(m_iCurHP);
		fHP = fCurHP / fMaxHP;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		_float fOriginWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fWidth = fOriginWidth * fHP; // HPBar 남은 길이
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		_float fRatio = _float(WINCY) / _float(WINCX);

		if (m_eUIType == BOSSHP::UI_FRAME)
		{
			_vec3 vScale = _vec3(fOriginWidth * fRatio * 0.8f, fHeight * fRatio * 0.6f, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			m_pTransformCom->Set_Pos(&m_vDefaultPos);
		}

		else if (m_eUIType == BOSSHP::UI_BACK)
		{
			_vec3 vScale = _vec3(fOriginWidth * fRatio * 4.17f, fHeight * fRatio, 0.f);
			m_pTransformCom->Set_Scale(vScale);
			m_pTransformCom->Set_Pos(&m_vDefaultPos);
		}

		else if (m_eUIType == BOSSHP::UI_GAUGE)
		{
			_float fX = fOriginWidth - fWidth;
			_vec3 vScale = _vec3(fWidth * fRatio * 4.2f, fHeight * fRatio, 0.f);
			m_pTransformCom->Set_Scale(vScale);

			if (fMaxHP == fCurHP)
			{
				m_pTransformCom->Set_Pos(&m_vDefaultPos);
			}
			else if (fCurHP < fMaxHP && fCurHP > 0)
			{
				_vec3 vMovePos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11),
					((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };
				m_pTransformCom->Set_Pos(&vMovePos);
			}

		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

		// Boss에 따른 이름 출력 : Test
		RECT rc = { 0, 0, WINCX, WINCY / 9 };
		TCHAR szBuf[256] = L"";
		swprintf_s(szBuf, L"시련의 원숭이 석상");

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

HRESULT CUI_BossHP::Add_Component(void)
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

	switch (m_eUIType)
	{
	case BOSSHP::UI_FRAME:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Boss_HPFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case BOSSHP::UI_BACK:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case BOSSHP::UI_GAUGE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_Boss_HPGauge"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	default:
		break;
	}

	return S_OK;
}

void CUI_BossHP::Set_Type(BOSSHP eType)
{
	m_eUIType = eType;
}

CUI_BossHP* CUI_BossHP::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOSSHP eType)
{
	CUI_BossHP* pInstance = new CUI_BossHP(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Boss HP Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_BossHP::Free()
{
}
