#include "UI_NewQuest.h"
#include "Export_Function.h"

CUI_NewQuest::CUI_NewQuest(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_NewQuest::CUI_NewQuest(const CUI_NewQuest& rhs) : CUI(rhs)
{
}

CUI_NewQuest::~CUI_NewQuest()
{
}

HRESULT CUI_NewQuest::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fX = WINCX / 2.f;
	m_tInfo.fY = WINCY / 2.f;

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height * 0.8f;

	m_fCurWidth = 64.f;
	m_fCurHeight = 43.f;

//	m_pTitleBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_TITLE);
//	FAILED_CHECK_RETURN(m_pTitleBox, E_FAIL);
//	m_pContentsBox = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CONTENTS);
//	FAILED_CHECK_RETURN(m_pContentsBox, E_FAIL);
//	m_pExclamIcon = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_ICON);
//	FAILED_CHECK_RETURN(m_pExclamIcon, E_FAIL);
//	m_pCloseKey = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CLOSE);
//	FAILED_CHECK_RETURN(m_pCloseKey, E_FAIL);

	return S_OK;
}

_int CUI_NewQuest::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	_float fRatio = _float(WINCY) / _float(WINCX);

	m_fMaxWidth = fWidth * fRatio * 2.1f;
	m_fMaxHeight = fHeight * fRatio * 2.1f;

	if (m_fCurWidth < m_fMaxWidth)
		m_fCurWidth += m_fCurWidth * fRatio * fTimeDelta * m_fSpeed;

	if (m_fCurHeight < m_fMaxHeight)
		m_fCurHeight += m_fCurHeight * fRatio * fTimeDelta * m_fSpeed;

	if (m_bShown)
	{
		m_pCloseKey->Update_Object(fTimeDelta);
		m_pTitleBox->Update_Object(fTimeDelta);
		m_pContentsBox->Update_Object(fTimeDelta);
		m_pExclamIcon->Update_Object(fTimeDelta);
	}

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_NewQuest::LateUpdate_Object(void)
{
	if (m_bShown)
	{
		m_pCloseKey->LateUpdate_Object();
		m_pTitleBox->LateUpdate_Object();
		m_pContentsBox->LateUpdate_Object();
		m_pExclamIcon->LateUpdate_Object();
	}

	__super::LateUpdate_Object();
}

void CUI_NewQuest::Render_Object(void)
{
	CUI::Render_Object();

	float fTimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) ,
				((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);

	_vec3 vScale = _vec3(m_fCurWidth, m_fCurHeight, 0.f);
	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	if (m_bShown &&
		(m_fCurHeight == m_fMaxHeight) &&
		(m_fCurWidth == m_fMaxWidth))
	{
		m_pCloseKey->Render_Object();
		m_pTitleBox->Render_Object();
		m_pContentsBox->Render_Object();
		m_pExclamIcon->Render_Object();
	}
}

HRESULT CUI_NewQuest::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_NoticeFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CUI_NewQuest* CUI_NewQuest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NewQuest* pInstance = new CUI_NewQuest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("New Quest Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NewQuest::Free()
{
}


