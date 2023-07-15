#include "EtcItem.h"
#include    "Export_Function.h"
#include    "Pool.h"
#include	"InventoryMgr.h"

CEtcItem::CEtcItem(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID) : CItem(pGraphicDev, ITEM_TYPE::ETC, _eID)
{
}

CEtcItem::CEtcItem(const CItem& rhs) : CItem(rhs)
{
}

CEtcItem::~CEtcItem()
{
}

HRESULT CEtcItem::Ready_Object(void)
{
	FAILED_CHECK(Add_Component());

    return S_OK;
}

_int CEtcItem::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);

	return __super::Update_Object(fTimeDelta);

}

void CEtcItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CEtcItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

CGameObject* CEtcItem::Get_ByPool()
{
    return CPool<CEtcItem>::Get_Obj();
}

void CEtcItem::Add_Pool()
{
	CPool<CEtcItem>::Return_Obj(this);
}

void CEtcItem::Free()
{
	__super::Free();
}

HRESULT CEtcItem::Change_Item(const ITEM_CODE& pCode)
{
	Safe_Release(m_pTextureCom);
	m_mapComponent[ID_STATIC].erase(COMPONENT_TYPE::COM_TEXTURE);

	wstring src = L"Proto_Texture_Item_";

	src += CItem::Get_ImgName(pCode);

	CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(src.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE , pComponent });

	m_eCode = pCode;

	return S_OK;

}

void CEtcItem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
		CInventoryMgr::GetInstance()->Add_Item(this);
}

CEtcItem* CEtcItem::Create(LPDIRECT3DDEVICE9 pGraphicDev,  OBJ_ID _eID, const ITEM_CODE& pCode)
{
	CEtcItem* ret = new CEtcItem(pGraphicDev, OBJ_ID::ITEM);

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

HRESULT CEtcItem::Add_Component(void)
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
