#include "UI_MapName.h"
#include "Export_Function.h"

CUI_MapName::CUI_MapName(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_fAlpha(155.f), m_fMaxWidth(0.f)
{
}

CUI_MapName::CUI_MapName(const CUI_MapName& rhs)
	: CUI(rhs), m_fAlpha(rhs.m_fAlpha), m_fMaxWidth(rhs.m_fMaxWidth)
{
}

CUI_MapName::~CUI_MapName()
{
}

HRESULT CUI_MapName::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_fSusTime = 0.f;

	m_tInfo.fX = WINCX / 2;
	m_tInfo.fY = WINCY / 2 - 250.f;

	m_fMaxWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width) * 0.6f;

	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height) * 0.35f;

	return S_OK;
}

_int CUI_MapName::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	if (!m_bIsMax)
		if (m_tInfo.fCX < m_fMaxWidth)
			m_tInfo.fCX += m_tInfo.fCX * 10.f * fTimeDelta;

	if (m_fAlpha > 0.f)
	{
		if( m_tInfo.fCX > m_fMaxWidth)
			m_bIsMax = true;

		if (m_bIsMax)
		{
			m_fAlpha -= 35.f * fTimeDelta;

			if (m_fSusTime > 40.f)
				m_tInfo.fCX -= m_tInfo.fCX * fTimeDelta;
		}
	}

	m_fSusTime += 10.f * fTimeDelta;

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_MapName::LateUpdate_Object(void)
{
	if (m_fAlpha <= 0.f)
		Set_Active(false);

	__super::LateUpdate_Object();
}

void CUI_MapName::Render_Object(void)
{
	_vec3 vPos, vScale;
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11),
	((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	_float fRatio = _float(WINCY) / _float(WINCX);

	vScale = _vec3(m_tInfo.fCX * fRatio, m_tInfo.fCY * fRatio, 0.f);

	m_pTransformCom->Set_Pos(&vPos);
	m_pTransformCom->Set_Scale(vScale);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_fAlpha + 40), 255, 255, 255));
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(155, 255, 255, 255));

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT rc = { 0, 150, WINCX, WINCY / 4 };
//	RECT rc = { 0, 0, WINCX, WINCY / 10 };
	TCHAR szBuf[256] = L"";

	switch (m_eSceneType)
	{
	case SCENE_TYPE::TUTORIAL_VILLAGE:
		swprintf_s(szBuf, L"오구 마을");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::MONKEY_FOREST1:
		swprintf_s(szBuf, L"조용한 숲길");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::MONKEY_FOREST2:
		swprintf_s(szBuf, L"신전 앞 동굴");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::MONKEY_FOREST3:
		swprintf_s(szBuf, L"시련의 숲");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::MONKEY_VILLAGE:
		swprintf_s(szBuf, L"원숭이 마을");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::SUNGOLEM_CAVE1:
		swprintf_s(szBuf, L"태양의 신전 : 골렘사원");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::SILK_WORM:
		swprintf_s(szBuf, L"나방 서식지 : 뽕나무 근처");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	case SCENE_TYPE::MOON_FOREST1:
		swprintf_s(szBuf, L"달의 숲");
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(_uint(m_fAlpha), 255, 255, 255));
		break;

	default:
		break;
	}

	__super::Render_Object();
}

HRESULT CUI_MapName::Ready_Component()
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_UI_MapName"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CUI_MapName* CUI_MapName::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE eType)
{
	CUI_MapName* pInstance = new CUI_MapName(pGraphicDev);

	pInstance->Set_SceneType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI MapName Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_MapName::Free()
{
	__super::Free();
}
