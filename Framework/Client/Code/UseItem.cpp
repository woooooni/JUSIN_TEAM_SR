#include "UseItem.h"
#include	"Export_Function.h"

CUseItem::CUseItem(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID) : CItem(pGraphicDev, ITEM_TYPE::CONSUMPTION, _eID)
{
}

CUseItem::CUseItem(const CItem& rhs) : CItem(rhs)
{
}

CUseItem::~CUseItem()
{
}

HRESULT CUseItem::Ready_Object(void)
{
    return E_NOTIMPL;
}

_int CUseItem::Update_Object(const _float& fTimeDelta)
{
    return _int();
}

void CUseItem::LateUpdate_Object(void)
{
}

void CUseItem::Render_Object(void)
{
}

CGameObject* CUseItem::Get_ByPool()
{
    return nullptr;
}

void CUseItem::Add_Pool()
{
}

void CUseItem::Free()
{
}


HRESULT CUseItem::Change_Item(const ITEM_CODE& pCode)
{
    return E_NOTIMPL;
}

void CUseItem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CUseItem* CUseItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID, const ITEM_CODE& pCode)
{
	
	CUseItem* ret = new CUseItem(pGraphicDev, OBJ_ID::ITEM);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("EtcItem Create Failed");
		return nullptr;
	}

	wstring src = L"Proto_Texture_Item_";

	src += CItem::Get_ImgName(pCode);

	CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(src.c_str()));
	if (!pComponent)
	{
		Safe_Release(ret);
		MSG_BOX("EtcItem Create Failed");
		return nullptr;
	}
	pComponent->SetOwner(ret);
	ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	ret->m_eCode = pCode;
	return ret;

}



HRESULT CUseItem::Use_Item()
{
	return E_NOTIMPL;
}

	HRESULT CUseItem::Add_Component(void)
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

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);


	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	m_pTransformCom->Set_Scale({ 0.8f, 0.8f, 1.f });

	Set_MinHeight(0.4f);


	return S_OK;
}
