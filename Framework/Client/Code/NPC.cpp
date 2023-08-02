#include "Export_Function.h"
#include "../Header/Npc.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"
#include "UI_ContinueMark.h"
#include "QuestMgr.h"
#include "Quest.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "UI_Dialog.h"
CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDev, NPC_CODE eCode, wstring _strNpcName)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_NPC, OBJ_ID::NPC) // OBJ_NPC
	, m_eCode(eCode)
	, m_strNpcName(_strNpcName)
	, m_bTalkEnble(true)
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

	m_pContinue = CUI_ContinueMark::Create(m_pGraphicDev);
	if (m_pContinue != nullptr)
		m_pContinue->Set_Owner(this);

	return S_OK;
}

_int CNpc::Update_Object(const _float& fTimeDelta)
{
	
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	_int iExit = __super::Update_Object(fTimeDelta);

	

	return iExit;
}

void CNpc::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	_float fTimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
	const vector<CQuest*>& vecQuest = CQuestMgr::GetInstance()->Get_QuestVec(m_eCode);

	_bool bBeforeQuest = false, bContinueQuest = false, bCompleteQuest = false;

	for (size_t i = 0; i < vecQuest.size(); ++i)
	{
		if (!bBeforeQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::BEFORE)
			bBeforeQuest = true;
		if (!bContinueQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::CONTINUE)
			bContinueQuest = true;
		if (!bCompleteQuest && vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::COMPLETE)
			bCompleteQuest = true;
	}

	if (bCompleteQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pQuestion->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
		m_pQuestion->Update_Object(fTimeDelta);
	}
	else if (bContinueQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pContinue->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
		m_pContinue->Update_Object(fTimeDelta);
	}
	else if (bBeforeQuest)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 1.f;
		m_pExclamation->Get_TransformCom()->Set_Info(INFO_POS, &vPos);
		m_pExclamation->Update_Object(fTimeDelta);
	}
	else
	{
		if (m_pExclamation)
			m_pExclamation->Set_Active(false);
		if (m_pQuestion)
			m_pQuestion->Set_Active(false);
		if (m_pContinue)
			m_pContinue->Set_Active(false);
	}

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	if (pPlayer != nullptr && (bBeforeQuest || bContinueQuest || bCompleteQuest))
	{
		_vec3 vPos, vPlayerPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vDir = vPos - vPlayerPos;
		if (D3DXVec3Length(&vDir) < 2.f)
		{
			CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(true);
			if (KEY_TAP(KEY::Z) 
				&& !CUIMgr::GetInstance()->Get_Dialog()->Is_Active() 
				&& !CUIMgr::GetInstance()->Get_NewQuestUI()->Is_Active() 
				&& !pPlayer->Is_GetItem()
				&& m_bTalkEnble)
			{
				Talk();

				Stop_Sound(CHANNELID::SOUND_UI);
				Play_Sound(L"SFX_69_UISlotBigMove.wav", CHANNELID::SOUND_UI, 0.9f);
			}
		}
		else
		{
			CUI_Dialog* pDialog = CUIMgr::GetInstance()->Get_Dialog();
			pDialog->Set_Active(false);
			CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
		}
	}

	if (!m_bTalkEnble)
		m_bTalkEnble = true;
}

void CNpc::Render_Object(void)
{
	
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

void CNpc::Talk()
{
	CQuest* pBegin = nullptr; 
	CQuest* pContinue = nullptr; 
	CQuest* pComplete = nullptr;

	const vector<CQuest*>& vecQuest = CQuestMgr::GetInstance()->Get_QuestVec(m_eCode);
	for (size_t i = 0; i < vecQuest.size(); ++i)
	{
		if (vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::BEFORE)
		{
			pBegin = vecQuest[i];
		}
		if (vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::CONTINUE)
		{
			pContinue = vecQuest[i];
		}
		if (vecQuest[i]->Get_Quest_Progress() == QUEST_PROGRESS::COMPLETE)
		{
			pComplete = vecQuest[i];
		}
	}

	CUI_Dialog* pDialog = CUIMgr::GetInstance()->Get_Dialog();
	if (nullptr != pComplete)
	{
		pDialog->Set_Name(m_strNpcName);
		pDialog->Set_Quest(pComplete);
		pDialog->Set_Active(true);
		return;
	}

	if (nullptr != pBegin)
	{
		pDialog->Set_Name(m_strNpcName);
		pDialog->Set_Quest(pBegin);
		pDialog->Set_Active(true);
		return;
	}

	if (nullptr != pContinue)
	{
		pDialog->Set_Name(m_strNpcName);
		pDialog->Set_Quest(pContinue);
		pDialog->Set_Active(true);
		return;
	}
}


void CNpc::Free()
{
	if (m_pExclamation)
		Safe_Release(m_pExclamation);

	if (m_pQuestion)
		Safe_Release(m_pQuestion);

	if (m_pContinue)
		Safe_Release(m_pContinue);

	__super::Free();
}
