#include "Npc_Sheep.h"
#include "Export_Function.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"

CNpc_Sheep::CNpc_Sheep(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_SHEEP, L"¾ç ¾ÆÁÜ¸¶")
{
}

CNpc_Sheep::CNpc_Sheep(const CNpc_Sheep& rhs)
	: CNpc(rhs)
{
}

CNpc_Sheep::~CNpc_Sheep()
{
}

HRESULT CNpc_Sheep::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
	if(pBoxCollider != nullptr)
		pBoxCollider->Set_Scale(_vec3(2.f, 2.f, 2.f));

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_Idle", L"Proto_Texture_NPC_Sheep_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_React", L"Proto_Texture_NPC_Sheep_React", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Sheep_Idle", TRUE), E_FAIL);

	m_pExclamation = CUI_ExclamationMark::Create(m_pGraphicDev);
	if (m_pExclamation != nullptr)
		m_pExclamation->Set_Owner(this);

	m_pQuestion = CUI_QuestionMark::Create(m_pGraphicDev);
	if (m_pQuestion != nullptr)
		m_pQuestion->Set_Owner(this);

	return S_OK;
}

_int CNpc_Sheep::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_vec3 vNpcPos;
	m_pTransformCom->Get_Info(INFO_POS, &vNpcPos);

	vNpcPos.y += 1.f;
	m_pExclamation->Get_TransformCom()->Set_Pos(&vNpcPos);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Sheep::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_Sheep::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

HRESULT CNpc_Sheep::Ready_Component()
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
}

CNpc_Sheep* CNpc_Sheep::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Sheep* pInstance = new CNpc_Sheep(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Sheep Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Sheep::Free()
{
	__super::Free();
}

