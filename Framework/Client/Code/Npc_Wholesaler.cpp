#include "../Include/stdafx.h"
#include "Npc_Wholesaler.h"
#include "Export_Function.h"

CNpc_Wholesaler::CNpc_Wholesaler(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_WHOLESALER)
{
}

CNpc_Wholesaler::CNpc_Wholesaler(const CNpc_Wholesaler& rhs)
	: CNpc(rhs)
{
}

CNpc_Wholesaler::~CNpc_Wholesaler()
{
}

HRESULT CNpc_Wholesaler::Ready_Object(void)
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_WholeSaler_Idle", L"Proto_Texture_NPC_Wholesaler_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_WholeSaler_Idle", TRUE), E_FAIL);

	return S_OK;
}

_int CNpc_Wholesaler::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Wholesaler::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_Wholesaler::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_Wholesaler* CNpc_Wholesaler::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Wholesaler* pInstance = new CNpc_Wholesaler(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC WholeSaler Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Wholesaler::Free()
{
}

