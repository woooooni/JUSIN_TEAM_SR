#include "DefaultItem.h"
#include    "Export_Function.h"
#include    "Pool.h"
#include	"InventoryMgr.h"
#include	"Player.h"

CDefaultItem::CDefaultItem(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID) : CItem(pGraphicDev, ITEM_TYPE::ETC, _eID)
{
}

CDefaultItem::CDefaultItem(const CItem& rhs) : CItem(rhs)
{
}

CDefaultItem::~CDefaultItem()
{
}

HRESULT CDefaultItem::Ready_Object(void)
{
	FAILED_CHECK(Add_Component());

    return S_OK;
}

_int CDefaultItem::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);

	return __super::Update_Object(fTimeDelta);

}

void CDefaultItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDefaultItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CDefaultItem::Use_Item()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player"));
	if (!pPlayer)
		return E_FAIL;

	switch (m_eCode)
	{
	case Engine::ITEM_CODE::HP_SMALL:
		pPlayer->Add_HP(1);
		break;
	case Engine::ITEM_CODE::HP_MIDDLE:
		pPlayer->Add_HP(3);

		break;
	case Engine::ITEM_CODE::HP_BIG:
		pPlayer->Add_HP(5);
		break;
	case Engine::ITEM_CODE::SPEED_SMALL:
		break;
	case Engine::ITEM_CODE::SPEED_MIDDLE:
		break;
	case Engine::ITEM_CODE::SPEED_BIG:
		break;

	default:
		return E_FAIL;
	}

	Set_InvenCount(false);

	if (Get_InvenCount() == 0)
		Add_Pool();

	return S_OK;
}


CGameObject* CDefaultItem::Get_ByPool()
{
    return CPool<CDefaultItem>::Get_Obj();
}

void CDefaultItem::Add_Pool()
{
	CPool<CDefaultItem>::Return_Obj(this);
}

void CDefaultItem::Free()
{
	__super::Free();
}

HRESULT CDefaultItem::Change_Item(const ITEM_CODE& pCode)
{
	if (pCode >= ITEM_CODE::ITEM_END)
		return E_FAIL;


	m_eCode = pCode;

	m_pTextureCom->Set_Idx((_uint)m_eCode);

	return S_OK;


}

void CDefaultItem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
		CInventoryMgr::GetInstance()->Add_Item(this);
}

CDefaultItem* CDefaultItem::Create(LPDIRECT3DDEVICE9 pGraphicDev,  OBJ_ID _eID, const ITEM_CODE& pCode)
{
	if (pCode < ITEM_CODE::HP_SMALL || pCode >= ITEM_CODE::ITEM_END)
		return nullptr;

	CDefaultItem* ret = new CDefaultItem(pGraphicDev, OBJ_ID::ITEM);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("DefaultItem Create Failed");
		return nullptr;
	}

	ret->m_eCode = pCode;
	ret->m_pTextureCom->Set_Idx((_uint)pCode);

	if (pCode >= ITEM_CODE::HP_SMALL && pCode < ITEM_CODE::CONSUME_END)
	{
		ret->m_eItemType = ITEM_TYPE::CONSUMPTION;
	}
	else if (pCode >= ITEM_CODE::LEAF && pCode < ITEM_CODE::ETC_END)
	{
		ret->m_eItemType = ITEM_TYPE::ETC;
	}
	return ret;
}

HRESULT CDefaultItem::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	m_pTransformCom->Set_Scale({ 0.8f, 0.8f, 1.f });

	Set_MinHeight(0.4f);


    return S_OK;
}
