#include "UI_Shop.h"
#include "Export_Function.h"

CUI_Shop::CUI_Shop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_Shop::CUI_Shop(const CUI_Shop& rhs)
	: CUI(rhs)
{
}

CUI_Shop::~CUI_Shop()
{
}

HRESULT CUI_Shop::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fX = -178.f;
	m_tInfo.fY = 70.f;

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

//	CGameObject* pCursur = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"UI_Shop_Cursor");
//	m_Cursor = pCursur;
	
	//bool bShown = dynamic_cast<CUI_ShortCutKey*>(pUI)->Get_Shown();

	return S_OK;
}

_int CUI_Shop::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	Key_Input();

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_Shop::LateUpdate_Object(void)
{
	//if (m_bShown)
	//	Set_SelectCursor();

	__super::LateUpdate_Object();
}

void CUI_Shop::Render_Object(void)
{
	if (m_bShown)
	{
		_matrix matPreView, matPreProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

		_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX ) * (1 / m_matProj._11) ,
						((-2 * (m_tInfo.fY)) / WINCY) * (1 / m_matProj._22), 0.f };
		m_pTransformCom->Set_Pos(&vPos);

		_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(fWidth * fRatio * 2.f, fHeight * fRatio * 2.f, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();
	}

}

HRESULT CUI_Shop::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Background"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	return S_OK;
}

void CUI_Shop::Key_Input()
{

	if (KEY_TAP(KEY::L))
	{
		if (!m_bShown)
			m_bShown = true;

		else
			m_bShown = false;
	}

}

void CUI_Shop::Set_Item()
{
	// 아이템 리스트가 비어있으면 return
//	if (m_ItemList.empty())
//		return;
//
//	auto& iter = m_ItemList.begin();
// 
//	switch (m_ItemList.size())
//	{
//	case 1:
//		(*iter)->Get_TransformCom()->Set_Pos();
//		break;
//
//	case 2:
//		(*iter)->Get_TransformCom()->Set_Pos();
//		iter++;
//		(*iter)->Get_TransformCom()->Set_Pos();
//		break;
//
//	case 3:
//		break;
// 
// 	 case 15까지 만들것 (1/1페이지 기준)
//
//	default:
//		break;
//	}

}

CUI_Shop* CUI_Shop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Shop* pInstance = new CUI_Shop(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Shop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Shop::Free()
{
}
