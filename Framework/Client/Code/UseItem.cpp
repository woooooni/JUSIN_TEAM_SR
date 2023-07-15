#include "UseItem.h"
#include	"Export_Function.h"
#include "Pool.h"
#include	"Player.h"
#include	"InventoryMgr.h"

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
	FAILED_CHECK(Add_Component());

	return S_OK;

}

_int CUseItem::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);

	return __super::Update_Object(fTimeDelta);

}

void CUseItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}

void CUseItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

}

CGameObject* CUseItem::Get_ByPool()
{
	return CPool<CUseItem>::Get_Obj();
}

void CUseItem::Add_Pool()
{
	CPool<CUseItem>::Return_Obj(this);
}

void CUseItem::Free()
{
	__super::Free();
}


HRESULT CUseItem::Change_Item(const ITEM_CODE& pCode)
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

void CUseItem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
		CInventoryMgr::GetInstance()->Add_Item(this);

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

HRESULT CUseItem::Use_Item(CPlayer* pPlayer)
{
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
