#include "Export_Function.h"
#include "../Header/Npc.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"
#include "QuestMgr.h"
#include "Quest.h"
CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDev, NPC_CODE eCode)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_INTERACTION, OBJ_ID::NPC) // OBJ_NPC
	, m_eCode(eCode)
{
}

CNpc::CNpc(const CNpc& rhs)
	:CGameObject(rhs)
{
}

CNpc::~CNpc()
{
}

HRESULT CNpc::Ready_Object(void)
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

	return S_OK;
}

_int CNpc::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	CQuest* pQuest = nullptr;
	vector<CQuest*> vecQuest = CQuestMgr::GetInstance()->Get_QuestVec(m_eCode);

	_bool bBeforQuest = false, bContinueQuest = false, bCompleteQuest = false;

	for (size_t i = 0; i < vecQuest.size(); ++i)
	{
		if (!bBeforQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::BEFORE)
			bBeforQuest = true;
		if (!bContinueQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::CONTINUE)
			bContinueQuest = true;
		if (!bContinueQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::COMPLETE)
			bContinueQuest = true;
	}

	if (bCompleteQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pQuestion->Update_Object(fTimeDelta);
	}
	else if (bContinueQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pQuestion->Update_Object(fTimeDelta);
	}
	else if (bBeforQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pExclamation->Update_Object(fTimeDelta);
	}



	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNpc::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}


void CNpc::Free()
{
	__super::Free();
}
