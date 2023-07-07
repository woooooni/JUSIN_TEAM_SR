#include "NPCText.h"
#include "Export_Function.h"

CNPCText::CNPCText(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CNPCText::CNPCText(const CNPCText& rhs)
	: CUI(rhs)
{
}

CNPCText::~CNPCText()
{
}

HRESULT CNPCText::Ready_Object(void)
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
	
	return S_OK;
}

_int CNPCText::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	// Key_Input 만들어서 넣기 (대화창 닫히게끔)

	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CNPCText::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNPCText::Render_Object(void)
{
	_matrix matPreView, matPreProj;
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);
	
	_vec3 vPos = { ((WINCX) * (1 / m_matProj._11)),
				  ((WINCY) * (1 / m_matProj._22)), 0.f };
	
	m_pTransformCom->Set_Pos(&vPos);
	
	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
	
	_float fRatio = _float(WINCY) / _float(WINCX);
	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
	
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

CNPCText* CNPCText::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNPCText* pInstance = new CNPCText(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC TextBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNPCText::Free()
{
}
