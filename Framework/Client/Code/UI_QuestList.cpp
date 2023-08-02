#include "UI_QuestList.h"
#include "Export_Function.h"
#include "UIMgr.h"

CUI_QuestList::CUI_QuestList(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_QuestList::CUI_QuestList(const CUI_QuestList& rhs)
	: CUI(rhs)
{
}

CUI_QuestList::~CUI_QuestList()
{
}

HRESULT CUI_QuestList::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	return S_OK;
}

_int CUI_QuestList::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	if (!CUIMgr::GetInstance()->Get_NewQuestUI()->Is_Active()) // QuestUI가 떠있지 않으면
		m_bShown = true;

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_QuestList::LateUpdate_Object(void)
{
	if (CUIMgr::GetInstance()->Get_NewQuestUI()->Is_Active())
		m_bShown = false;

	if (CUIMgr::GetInstance()->Get_ShopUI()->Get_Shown())
		m_bShown = false;

	if (CUIMgr::GetInstance()->Get_ShopUpdate())
		m_bShown = false;

	if (CUIMgr::GetInstance()->Get_Dialog()->Is_Active())
		m_bShown = false;

	__super::LateUpdate_Object();
}

void CUI_QuestList::Render_Object(void)
{
	if (m_bShown)
	{

		_matrix matPreView, matPreProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		_vec3 vPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };

		m_pTransformCom->Set_Pos(&vPos);

		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(_float(m_tInfo.fCX * fRatio * 0.75f), _float(m_tInfo.fCY * fRatio * 0.6f), 0.f);
		m_pTransformCom->Set_Scale(vScale);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(150, 255, 255, 255));

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		// Quest 내용 RC
		RECT rcName = { WINCX * 2 / 3 + 50, WINCY * 3 / 4 + 55, WINCX, WINCY };

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD_SMALL)->DrawText(NULL,
			m_strTitle.c_str(), INT(m_strTitle.size()), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(180, 255, 255, 255));

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

		__super::Render_Object();

	}
}

HRESULT CUI_QuestList::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_QuestList"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	m_tInfo.fX = WINCX / 2 + 450.f;
	m_tInfo.fY = WINCY / 2 + 350.f;

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	return S_OK;
}

CUI_QuestList* CUI_QuestList::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_QuestList* pInstance = new CUI_QuestList(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Quest List Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_QuestList::Free()
{
}

