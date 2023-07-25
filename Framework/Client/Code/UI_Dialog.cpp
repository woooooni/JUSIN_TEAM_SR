#include "UI_Dialog.h"
#include "Export_Function.h"

CUI_Dialog::CUI_Dialog(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_bShown(false)
	, m_strName(L"")
	, m_strDesc(L"")
	, m_strCurrDesc(L"")
	, m_fAccTime(0.f)
	, m_fDescTime(5.f)
	, m_iIdx(0)
{

}


CUI_Dialog::CUI_Dialog(const CUI_Dialog& rhs)
	: CUI(rhs)
	, m_bShown(false)
	, m_strName(L"")
	, m_strDesc(L"")
	, m_strCurrDesc(L"")
	, m_fAccTime(0.f)
	, m_fDescTime(5.f)
	, m_iIdx(0)
{

}

CUI_Dialog::~CUI_Dialog()
{
}

HRESULT CUI_Dialog::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	return S_OK;
}

_int CUI_Dialog::Update_Object(const _float& fTimeDelta)
{
	if (!m_bShown)
		return S_OK;

	m_fAccTime += fTimeDelta;
	if (m_fAccTime >= m_fDescTime)
	{
		m_fAccTime = 0.f;
		m_iIdx++;

		if (m_iIdx >= _uint(m_strDesc.size()))
			m_iIdx = _uint(m_strDesc.size() - 1);

		m_strCurrDesc = m_strDesc.substr(0, m_iIdx);
	}

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CUI_Dialog::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_Dialog::Render_Object(void)
{
	if (m_bShown)
	{
		_matrix matPreView, matPreProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) * (1 / m_matProj._11) ,
					((-2 * (WINCY / 2)) / WINCY + 0.5f) * (1 / m_matProj._22), 0.f };

		m_pTransformCom->Set_Pos(&vPos);

		// 백그라운드 출력
		_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(_float(fWidth * fRatio * 0.8), _float(fHeight * fRatio), 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();


		// Name Tag 위치
		RECT rcName = { -1 * (WINCX / 4) + 70, (WINCY / 4) - 50, 3 * (WINCX / 4), WINCY };
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			m_strName.c_str(), INT(m_strName.size()), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(100, 0, 0, 0));

		// Text 출력 위치
		RECT rcText = { 0, WINCY / 2, WINCX, WINCY };
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			m_strCurrDesc.c_str(), INT(m_strCurrDesc.size()), &rcText, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(100, 0, 0, 0));

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	}

	__super::Render_Object();
}

HRESULT CUI_Dialog::Ready_Component()
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_TextBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fX = 0.f;
	m_tInfo.fY = 220.f;

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	
	return S_OK;
}


CUI_Dialog* CUI_Dialog::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Dialog* pInstance = new CUI_Dialog(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CUI_Dialog Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Dialog::Free()
{
	__super::Free();
}
