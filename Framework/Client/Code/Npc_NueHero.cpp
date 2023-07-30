#include "../Include/stdafx.h"
#include "Npc_NueHero.h"
#include "Export_Function.h"
#include "UI_ExclamationMark.h"
#include "UI_QuestionMark.h"
#include "UI_ContinueMark.h"

CNpc_NueHero::CNpc_NueHero(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_NUE_HERO, L"누에 용사")
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_NueHero_Idle", L"Proto_Texture_NPC_NueHero_Idle", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_NueHero_Idle", TRUE), E_FAIL);

	m_pExclamation = CUI_ExclamationMark::Create(m_pGraphicDev);
	if (m_pExclamation != nullptr)
		m_pExclamation->Set_Owner(this);

	m_pQuestion = CUI_QuestionMark::Create(m_pGraphicDev);
	if (m_pQuestion != nullptr)
		m_pQuestion->Set_Owner(this);

	m_pContinue = CUI_ContinueMark::Create(m_pGraphicDev);
	if (m_pContinue != nullptr)
		m_pContinue->Set_Owner(this);

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

	m_pAnimator->Render_Component();

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
