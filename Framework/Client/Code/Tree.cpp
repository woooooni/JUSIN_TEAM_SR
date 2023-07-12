#include "Tree.h"
#include "Export_Function.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphicDev)
    : CFixedObj(pGraphicDev, OBJ_ID::TREE)
{

}

CTree::CTree(const CTree& rhs)
    : CFixedObj(rhs)
{

}

CTree::~CTree()
{

}

HRESULT CTree::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	return S_OK;
}

_int CTree::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CTree::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTree::Render_Object(void)
{
	//Set_Billboard();
	__super::Render_Object();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CTree::Ready_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Tree"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	return S_OK;
}

CTree* CTree::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTree* pInstance = new CTree(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Tree Object Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTree::Free()
{
}
