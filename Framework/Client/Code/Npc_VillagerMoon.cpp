#include "Npc_VillagerMoon.h"
#include "../Include/stdafx.h"
#include "Export_Function.h"

CNpc_VillagerMoon::CNpc_VillagerMoon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev)
{
}

CNpc_VillagerMoon::CNpc_VillagerMoon(const CNpc_VillagerMoon& rhs)
	: CNpc(rhs)
{
}

CNpc_VillagerMoon::~CNpc_VillagerMoon()
{
}

HRESULT CNpc_VillagerMoon::Ready_Object(void)
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

	switch (m_eType)
	{
	case MOONVILLAGER::MOON_RED:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_MoonVillager_Red_Idle", L"Proto_Texture_NPC_MoonVillager_Red_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_MoonVillager_Red_Dance", L"Proto_Texture_NPC_MoonVillager_Red_Dance", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_MoonVillager_Red_Dance", TRUE), E_FAIL);
		break;

	case MOONVILLAGER::MOON_YELLOW:
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_MoonVillager_Yellow_Idle", L"Proto_Texture_NPC_MoonVillager_Yellow_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_MoonVillager_Yellow_React", L"Proto_Texture_NPC_MoonVillager_Yellow_React", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_MoonVillager_Yellow_React", TRUE), E_FAIL);
		break;

	default:
		break;
	}

	return S_OK;
}

_int CNpc_VillagerMoon::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_VillagerMoon::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_VillagerMoon::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

void CNpc_VillagerMoon::Set_Type(MOONVILLAGER eType)
{
	m_eType = eType;
}

CNpc_VillagerMoon* CNpc_VillagerMoon::Create(LPDIRECT3DDEVICE9 pGraphicDev, MOONVILLAGER eType)
{
	CNpc_VillagerMoon* pInstance = new CNpc_VillagerMoon(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Moon Villager Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_VillagerMoon::Free()
{
}
