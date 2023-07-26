#include "Npc_Artist.h"
#include "Export_Function.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"
#include "UI_ContinueMark.h"
CNpc_Artist::CNpc_Artist(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_ARTIST, L"∏Ù∆º¡Ó")
{
}

CNpc_Artist::CNpc_Artist(const CNpc_Artist& rhs)
	: CNpc(rhs)
{
}

CNpc_Artist::~CNpc_Artist()
{
}

HRESULT CNpc_Artist::Ready_Object(void)
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

	m_pExclamation = CUI_ExclamationMark::Create(m_pGraphicDev);
	if (m_pExclamation != nullptr)
		m_pExclamation->Set_Owner(this);

	m_pQuestion = CUI_QuestionMark::Create(m_pGraphicDev);
	if (m_pQuestion != nullptr)
		m_pQuestion->Set_Owner(this);

	m_pContinue = CUI_ContinueMark::Create(m_pGraphicDev);
	if (m_pContinue != nullptr)
		m_pContinue->Set_Owner(this);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Artist_Idle", L"Proto_Texture_NPC_Artist_Idle", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Artist_Drawing", L"Proto_Texture_NPC_Artist_Drawing", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Artist_React", L"Proto_Texture_NPC_Artist_React", 0.2f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Artist_Drawing", TRUE), E_FAIL);

	return S_OK;
}

_int CNpc_Artist::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Artist::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_Artist::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_Artist* CNpc_Artist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Artist* pInstance = new CNpc_Artist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Artist Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Artist::Free()
{
}
