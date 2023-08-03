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
	if (m_bIsInInven)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_ITEM);

	return __super::Update_Object(fTimeDelta);

}

void CDefaultItem::LateUpdate_Object(void)
{
	if (m_bIsInInven)
		return;
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
		pPlayer->Set_Player_SpeedUp(3.f);
		break;
	case Engine::ITEM_CODE::SPEED_MIDDLE:
		pPlayer->Set_Player_SpeedUp(6.f);

		break;
	case Engine::ITEM_CODE::SPEED_BIG:
		pPlayer->Set_Player_SpeedUp(10.f);

		break;
	case Engine::ITEM_CODE::LEAF:
		break;
	case Engine::ITEM_CODE::TWIG:
		break;
	case Engine::ITEM_CODE::BUTTERFLY:
		break;
	case Engine::ITEM_CODE::SASUM:
		break;
	case Engine::ITEM_CODE::HAT_DRILL:
		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::DRILL);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);
		break;
	case Engine::ITEM_CODE::HAT_LIGHT:
		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::LIGHT);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);

		break;
	case Engine::ITEM_CODE::HAT_MASK:
		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::LIGHTNING);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);

		break;
	case Engine::ITEM_CODE::HAT_MISSLE:
		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::MISSILE);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);

		break;
	case Engine::ITEM_CODE::HAT_MONKEY:

		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::GOLEMFIST);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);

		break;
	case Engine::ITEM_CODE::HAT_TURTLE:
		if (!pPlayer->Get_Hat() || pPlayer->Get_Hat()->Get_ItemCode() != m_eCode)
			pPlayer->Set_PlayerHat(PLAYER_HAT::TURTLE);
		else
			pPlayer->Set_PlayerHat(PLAYER_HAT::NONE);



		break;
	default:
		return E_FAIL;
	}

	if(m_eItemType == ITEM_TYPE::CONSUMPTION)
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
	{
		CPlayer* player = dynamic_cast<CPlayer*>(pCollider->GetOwner());

		if (player->Is_GetItem())
			return;

		CInventoryMgr::GetInstance()->Add_Item(this);

		Stop_Sound(CHANNELID::SOUND_EFFECT_SYSTEM);
		Play_Sound(L"SFX_551_Ogu_GetItem.wav", CHANNELID::SOUND_EFFECT_SYSTEM, 0.6f);
	}
}

void CDefaultItem::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
	{
		CPlayer* player = dynamic_cast<CPlayer*>(pCollider->GetOwner());

		if (player->Is_GetItem())
			return;

		CInventoryMgr::GetInstance()->Add_Item(this);

	}
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
	else if (pCode >= ITEM_CODE::HAT_DRILL && pCode < ITEM_CODE::EQUIP_END)
	{
		ret->m_eItemType = ITEM_TYPE::EQUIPMENT;
	}
	else if (pCode >= ITEM_CODE::DRAWING_CROSSROAD && pCode < ITEM_CODE::IMPORTANT_END)
	{
		ret->m_eItemType = ITEM_TYPE::IMPORTANT;
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
