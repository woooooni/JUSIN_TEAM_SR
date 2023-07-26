#include "../Include/stdafx.h"
#include "Npc_VillagerMonkey.h"
#include "Export_Function.h"

CNpc_VillagerMonkey::CNpc_VillagerMonkey(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_VILLAGER_MONKEY, L"ÁÖ¹Î ¿ø¼þÀÌ")
{
}

CNpc_VillagerMonkey::CNpc_VillagerMonkey(const CNpc_VillagerMonkey& rhs)
	: CNpc(rhs)
{
}

CNpc_VillagerMonkey::~CNpc_VillagerMonkey()
{
}

HRESULT CNpc_VillagerMonkey::Ready_Object(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);
	m_pTransformCom->Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
	switch (m_eType)
	{
	case VILLAGERTYPE::MONKEY_HOOD:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Villager_Hood", L"Proto_Texture_NPC_Villager_Hood_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Villager_Hood", TRUE), E_FAIL);
		break;
		
	case VILLAGERTYPE::MONKEY_FUR:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Villager_Fur", L"Proto_Texture_NPC_Villager_Fur_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Villager_Fur", TRUE), E_FAIL);
		break;

	case VILLAGERTYPE::MONKEY_LEAVES1:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Villager_Leaves1", L"Proto_Texture_NPC_Villager_Leaves1_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Villager_Leaves1", TRUE), E_FAIL);
		break;

	case VILLAGERTYPE::MONKEY_LEAVES2:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Villager_Leaves2", L"Proto_Texture_NPC_Villager_Leaves2_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Villager_Leaves2", TRUE), E_FAIL);
		break;

	case VILLAGERTYPE::MONKEY_DANCER:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Villager_Dancer", L"Proto_Texture_NPC_Villager_Dancer_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Villager_Dancer", TRUE), E_FAIL);
		break;

	default:
		break;
	}

	return S_OK;
}

_int CNpc_VillagerMonkey::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_VillagerMonkey::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_VillagerMonkey::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

void CNpc_VillagerMonkey::Set_Type(VILLAGERTYPE eType)
{
	m_eType = eType;
}

CNpc_VillagerMonkey* CNpc_VillagerMonkey::Create(LPDIRECT3DDEVICE9 pGraphicDev, VILLAGERTYPE eType)
{
	CNpc_VillagerMonkey* pInstance = new CNpc_VillagerMonkey(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Villager Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_VillagerMonkey::Free()
{
}
