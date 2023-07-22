#include "UI_QuestIcon.h"
#include "Export_Function.h"

CUI_QuestIcon::CUI_QuestIcon(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_QuestIcon::CUI_QuestIcon(const CUI_QuestIcon& rhs) : CUI(rhs)
{
}

CUI_QuestIcon::~CUI_QuestIcon()
{
}

HRESULT CUI_QuestIcon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

_int CUI_QuestIcon::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	if (!m_pOwner->Is_Active())
		Set_Active(false);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_QuestIcon::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_QuestIcon::Render_Object(void)
{
	if (m_bShown)
	{
		_matrix matPreView, matPreProj;
		_vec3	vPos, vScale;
		_float fWidth, fHeight, fRatio;
	
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
	
		switch (m_eIconType)
		{
		case QUESTICON::QUEST_TITLE:
			vPos = { (2 * (m_tInfo.fX + 420.f) / WINCX) * (1 / m_matProj._11) ,
					(-2 * (m_tInfo.fY + 150.f) / WINCY) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);
	
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 3.f, m_tInfo.fCY * fRatio * 3.f, 0.f);
			break;
	
		case QUESTICON::QUEST_CONTENTS:
			vPos = { (2 * (m_tInfo.fX + 420.f) / WINCX) * (1 / m_matProj._11) ,
					(-2 * (m_tInfo.fY + 340.f) / WINCY) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);
	
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.5f, m_tInfo.fCY * fRatio, 0.f);
			break;
	
		case QUESTICON::QUEST_ICON:
			vPos = { (2 * (m_tInfo.fX + 245.f) / WINCX) * (1 / m_matProj._11) ,
					(-2 * (m_tInfo.fY + 280.f) / WINCY) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);
	
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.6f, m_tInfo.fCY * 1.5 * fRatio, 0.f);
			break;
	
		case QUESTICON::QUEST_CLOSE:
			vPos = { (2 * (m_tInfo.fX + 1120.f) / WINCX) * (1 / m_matProj._11) ,
					(-2 * (m_tInfo.fY + 750.f) / WINCY) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);
	
			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 1.2f, m_tInfo.fCY * fRatio * 1.2f, 0.f);
			break;

		case QUESTICON::QUEST_SLOT:
			vPos = { (2 * (m_tInfo.fX) / WINCX) * (1 / m_matProj._11) ,
					(-2 * (m_tInfo.fY - 20.f) / WINCY) * (1 / m_matProj._22), 0.f };
			m_pTransformCom->Set_Pos(&vPos);

			fRatio = _float(WINCY) / _float(WINCX);
			vScale = _vec3(m_tInfo.fCX * fRatio * 2.5f, m_tInfo.fCY * fRatio * 2.5f, 0.f);
			break;
	
		default:
			break;
		}
	
		m_pTransformCom->Set_Pos(&vPos);
	
		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();
	
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	
		__super::Render_Object();
	}
}

HRESULT CUI_QuestIcon::Add_Component(void)
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

	switch (m_eIconType)
	{
	case QUESTICON::QUEST_TITLE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Quest_TitleBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = -420.f;
		m_tInfo.fY = -390.f;
		break;

	case QUESTICON::QUEST_CONTENTS:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Quest_ContentsBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = -420.f;
		m_tInfo.fY = -390.f;
		break;

	case QUESTICON::QUEST_CLOSE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Quest_CloseKey"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = -605.f;
		m_tInfo.fY = -390.f;
		break;

	case QUESTICON::QUEST_ICON:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_Quest"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = -605.f;
		m_tInfo.fY = -330.f;
		break;

	case QUESTICON::QUEST_SLOT:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_QuickSlot"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		m_tInfo.fX = 0.f;
		m_tInfo.fY = 0.f;
		break;

	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}

void CUI_QuestIcon::Set_Type(QUESTICON eType)
{
	m_eIconType = eType;
}

CUI_QuestIcon* CUI_QuestIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, QUESTICON eType)
{
	CUI_QuestIcon* pInstance = new CUI_QuestIcon(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("QuestIcon Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_QuestIcon::Free()
{
}
