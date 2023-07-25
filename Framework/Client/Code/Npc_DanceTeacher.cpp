#include "../Include/stdafx.h"
#include "Npc_DanceTeacher.h"
#include "Export_Function.h"
#include "UI_ExclamationMark.h"
#include "UI_QuestionMark.h"
#include "UI_ContinueMark.h"

CNpc_DanceTeacher::CNpc_DanceTeacher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_DANCETEACHER, L"Ãã ¼±»ý´Ô")
{
}

CNpc_DanceTeacher::CNpc_DanceTeacher(const CNpc_DanceTeacher& rhs)
	: CNpc(rhs)
{
}

CNpc_DanceTeacher::~CNpc_DanceTeacher()
{
}

HRESULT CNpc_DanceTeacher::Ready_Object(void)
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
	m_pTransformCom->Set_Scale(_vec3(1.65f, 1.65f, 1.65f));

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_DanceTeacher_Idle", L"Proto_Texture_NPC_DanceTeacher_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_DanceTeacher_Dance", L"Proto_Texture_NPC_DanceTeacher_Dance", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Monkey_DanceTeacher_React", L"Proto_Texture_NPC_DanceTeacher_React", 0.5f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Monkey_DanceTeacher_Dance", TRUE), E_FAIL);

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

_int CNpc_DanceTeacher::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_DanceTeacher::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_DanceTeacher::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_DanceTeacher* CNpc_DanceTeacher::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_DanceTeacher* pInstance = new CNpc_DanceTeacher(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC DanceTeacher Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_DanceTeacher::Free()
{
}
