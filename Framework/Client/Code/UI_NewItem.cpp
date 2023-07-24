#include "UI_NewItem.h"
#include "Export_Function.h"
#include "Item.h"
#include "GameMgr.h"
#include "Player.h"

CUI_NewItem::CUI_NewItem(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_NewItem::CUI_NewItem(const CUI_NewItem& rhs) : CUI(rhs)
{
}

CUI_NewItem::~CUI_NewItem()
{
}

HRESULT CUI_NewItem::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tInfo.fX = 0.f;
	m_tInfo.fY = -20.f;

	m_pWindow = CUI_Notification::Create(m_pGraphicDev, 0.8f, 0.79f);
	if (m_pWindow != nullptr)
		m_pWindow->Set_Owner(this);

	m_pFrame = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_SLOT);
	if (m_pFrame != nullptr)
		m_pFrame->Set_Owner(this);

	m_pCloseKey = CUI_QuestIcon::Create(m_pGraphicDev, QUESTICON::QUEST_CLOSE);
	if (m_pCloseKey != nullptr)
		m_pCloseKey->Set_Owner(this);

	m_pTextureCom->Set_Idx(0);
	Set_Active(false);

	return S_OK;
}

_int CUI_NewItem::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	m_pWindow->Update_Object(fTimeDelta);

	m_pCloseKey->Update_Object(fTimeDelta);
	m_pFrame->Update_Object(fTimeDelta);

	Key_Input();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_NewItem::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	m_pWindow->LateUpdate_Object();

	m_pCloseKey->LateUpdate_Object();
	m_pFrame->LateUpdate_Object();

	m_fCurWidth = m_pWindow->Get_CurWidth();
	m_fMaxWidth = m_pWindow->Get_MaxWidth();

	if (m_fCurWidth > m_fMaxWidth)
	{
		m_pCloseKey->Set_Shown(true);
		m_pFrame->Set_Shown(true);

		m_bShown = true;
	}

	__super::LateUpdate_Object();
}

void CUI_NewItem::Render_Object(void)
{
	if (!Is_Active())
		return;

	if (m_bShown)
	{
		// 아이템명
		RECT rcTitle = { 0, 100, WINCX, WINCY / 2 };
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL,
				m_strItemName.c_str(), m_strItemName.length(), &rcTitle,
				DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 아이템설명
		RECT rcContents = { 0, 290, WINCX, WINCY };
		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			m_strItemInfo.c_str(), m_strItemInfo.length(), &rcContents,
			DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 아이템 IMG
		_matrix matPreView, matPreProj;
		_vec3	vPos, vScale;
		_float fWidth, fHeight, fRatio;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
		
		vPos = { (2 * m_tInfo.fX / WINCX) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

		fRatio = _float(WINCY) / _float(WINCX);
		vScale = _vec3(m_tInfo.fCX * fRatio * 1.5f, m_tInfo.fCY * fRatio * 1.5f, 0.f);
		
		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	}

}

HRESULT CUI_NewItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CUI_NewItem::Key_Input()
{
	if (m_bShown)
		if (KEY_TAP(KEY::Z))
		{
			CGameObject* pPlayer = CGameMgr::GetInstance()->Get_Player();
			dynamic_cast<CPlayer*>(pPlayer)->Set_GetItem(false);

			Set_Active(false);
			m_bShown = false;

			m_pWindow->Set_Active(false);
			m_pWindow->Set_InitSize(64.f, 43.f);

			m_pCloseKey->Set_Active(false);
			m_pCloseKey->Set_Shown(false);

			m_pFrame->Set_Active(false);
			m_pFrame->Set_Shown(false);
		}
}

void CUI_NewItem::Set_Item(ITEM_CODE _eCodeType)
{
	m_strItemInfo = CItem::Get_Explain(_eCodeType);

	switch (_eCodeType)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		m_strItemName = L"건강 열매";
		break;

	case Engine::ITEM_CODE::HP_MIDDLE:
		m_strItemName = L"수입산 사과";
		break;

	case Engine::ITEM_CODE::HP_BIG:
		m_strItemName = L"국내산 사과";
		break;

	case Engine::ITEM_CODE::SPEED_SMALL:
		m_strItemName = L"신비한 약초";
		break;

	case Engine::ITEM_CODE::SPEED_MIDDLE:
		m_strItemName = L"더 신비한 약초";
		break;

	case Engine::ITEM_CODE::SPEED_BIG:
		m_strItemName = L"진짜 신비한 약초";
		break;

	case Engine::ITEM_CODE::LEAF:
		m_strItemName = L"나뭇잎";
		break;

	case Engine::ITEM_CODE::TWIG:
		m_strItemName = L"나뭇가지";
		break;

	case ITEM_CODE::BUTTERFLY:
		m_strItemName = L"장수풍뎅이 탈을 쓴 나비";
		break;

	case Engine::ITEM_CODE::ITEM_END:
		break;

	default:
		break;
	}
}

void CUI_NewItem::Get_ItemInfo(ITEM_CODE _eCodeType)
{
	Set_Active(true);
	m_pWindow->Set_Active(true);

	m_pTextureCom->Set_Idx((_uint)_eCodeType);

	Set_Item(_eCodeType); // 아이템 정보를 설정
}

CUI_NewItem* CUI_NewItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NewItem* pInstance = new CUI_NewItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NewItem Window Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_NewItem::Free()
{
	__super::Free();
}
