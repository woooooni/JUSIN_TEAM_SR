#include "Npc_OguMom.h"
#include "Export_Function.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"
#include "UI_ContinueMark.h"
CNpc_OguMom::CNpc_OguMom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev, NPC_CODE::NPC_MOM, L"엄마 오구")
{
}

CNpc_OguMom::CNpc_OguMom(const CNpc_OguMom& rhs)
	: CNpc(rhs)
{
}

CNpc_OguMom::~CNpc_OguMom()
{
}

HRESULT CNpc_OguMom::Ready_Object(void)
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_OguMom_Idle", L"Proto_Texture_NPC_OguMom_Idle", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_OguMom_Greeting", L"Proto_Texture_NPC_OguMom_Greeting", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_OguMom_React", L"Proto_Texture_NPC_OguMom_React", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_OguMom_Greeting", TRUE), E_FAIL);

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

_int CNpc_OguMom::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_OguMom::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc_OguMom::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	_vec3 vNpcPos;
	m_pTransformCom->Get_Info(INFO_POS, &vNpcPos);
	vNpcPos.y = 0.75f;
	m_pTransformCom->Set_Pos(&vNpcPos);

	_vec3 vSlotScale = _vec3(1.5f, 1.5f, 0.f);
	m_pTransformCom->Set_Scale(vSlotScale);

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNpc_OguMom* CNpc_OguMom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_OguMom* pInstance = new CNpc_OguMom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC OguMom Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_OguMom::Free()
{
}
