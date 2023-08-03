#include "UI_Mouse.h"
#include "Export_Function.h"
#include "UIMgr.h"

CUI_Mouse::CUI_Mouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CUI_Mouse::CUI_Mouse(const CUI_Mouse& rhs)
	: CUI(rhs)
{
}

CUI_Mouse::~CUI_Mouse()
{
}

HRESULT CUI_Mouse::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_tInfo.fCX = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	m_tInfo.fCY = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	m_pTransformCom->Set_Scale(_vec3(_float(m_tInfo.fCX), _float(m_tInfo.fCY), 1.f));

	return S_OK;
}

_int CUI_Mouse::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	m_tInfo.fX = _float(CKeyMgr::GetInstance()->GetMousePos().x);
	m_tInfo.fY = _float(CKeyMgr::GetInstance()->GetMousePos().y);

	return iExit;
}

void CUI_Mouse::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_Mouse::Render_Object(void)
{
	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) ,
				((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.001f };

	m_pTransformCom->Set_Pos(&vPos);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUI_Mouse::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Mouse"));
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

CUI_Mouse* CUI_Mouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Mouse* pInstance = new CUI_Mouse(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Quest List Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Mouse::Free()
{
}

