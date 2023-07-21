#include "../Include/stdafx.h"
#include "Npc_Dancer.h"
#include "Export_Function.h"

CNpc_Dancer::CNpc_Dancer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev)
{
}

CNpc_Dancer::CNpc_Dancer(const CNpc_Dancer& rhs)
	: CNpc(rhs)
{
}

CNpc_Dancer::~CNpc_Dancer()
{
}

HRESULT CNpc_Dancer::Ready_Object(void)
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_Dancer_Dance", L"Proto_Texture_NPC_Dancer_Dance", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_Dancer_Dance", TRUE), E_FAIL);
	
	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);
	m_fMinHeight = 0.5f;
	return S_OK;
}

_int CNpc_Dancer::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	
	_int iExit = __super::Update_Object(fTimeDelta);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	_int iIdx = m_pAnimator->GetCurrAnimation()->Get_Idx();
	if ( vPos.y <= 0.5f && (iIdx == 0 || iIdx == 6 || iIdx == 13 || iIdx == 20 || iIdx == 27))
		m_pRigidBodyCom->AddForce(_vec3(0.f, 5.f , 0.f));

	return iExit;
}

void CNpc_Dancer::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_Dancer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_Dancer* CNpc_Dancer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Dancer* pInstance = new CNpc_Dancer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC DancerMonkey Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Dancer::Free()
{
}
