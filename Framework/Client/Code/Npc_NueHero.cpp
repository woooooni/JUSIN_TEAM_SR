#include "../Include/stdafx.h"
#include "Npc_NueHero.h"
#include "Export_Function.h"

CNpc_NueHero::CNpc_NueHero(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev)
{
}

CNpc_NueHero::CNpc_NueHero(const CNpc_NueHero& rhs)
	: CNpc(rhs)
{
}

CNpc_NueHero::~CNpc_NueHero()
{
}

HRESULT CNpc_NueHero::Ready_Object(void)
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_NueHero_Idle", L"Proto_Texture_NPC_NueHero_Idle", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_NueHero_Idle", TRUE), E_FAIL);

	return S_OK;
}

_int CNpc_NueHero::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_NueHero::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_NueHero::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_NueHero* CNpc_NueHero::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_NueHero* pInstance = new CNpc_NueHero(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC NueHero Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_NueHero::Free()
{
}
