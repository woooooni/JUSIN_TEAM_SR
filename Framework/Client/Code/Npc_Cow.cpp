#include "../Include/stdafx.h"
#include "Npc_Cow.h"
#include "Export_Function.h"

CNpc_Cow::CNpc_Cow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev)
{
}

CNpc_Cow::CNpc_Cow(const CNpc_Cow& rhs)
	: CNpc(rhs)
{
}

CNpc_Cow::~CNpc_Cow()
{
}

HRESULT CNpc_Cow::Ready_Object(void)
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

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Cow_Idle", L"Proto_Texture_NPC_Cow_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Cow_React", L"Proto_Texture_NPC_Cow_React", 0.5f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Cow_Idle", TRUE), E_FAIL);

//	m_pExclamation = CUI_ExclamationMark::Create(m_pGraphicDev);
	//if (m_pExclamation != nullptr)
	//	m_pExclamation->Set_Owner(this);

	m_pQuestion = CUI_QuestionMark::Create(m_pGraphicDev);
	if (m_pQuestion != nullptr)
		m_pQuestion->Set_Owner(this);

	return S_OK;
}

_int CNpc_Cow::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	//CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	//CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	//CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	//CGameObject* pNPCSheep = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Sheep");

	//_vec3 vPlayerPos, vNPCPos;

	//bool bShown = dynamic_cast<CNpcTextBox*>(pUIBox)->Get_Shown();
	//bool bCol = dynamic_cast<CNpcSheep*>(pNPCSheep)->Get_Collision();

	//pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	//m_pTransformCom->Get_Info(INFO_POS, &vNPCPos);
	//_float fLength = D3DXVec3Length(&(vPlayerPos - vNPCPos));

	//if (fLength <= 2.f)
	//{
	//	bool bShown = dynamic_cast<CNpcTextBox*>(pUIBox)->Get_Shown();

	//	m_bCollision = true; // NPC 근처에 Player가 있다.
	//	dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(true); // 단축키 안내 On

	//	if (bShown) // TEXTBOX가 켜져있는 상황이라면 단축키 안내를 끈다.
	//		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	//}

	//else if (fLength > 2.f)
	//{
	//	m_bCollision = false;

	//	if (!bCol)
	//		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	//}

	_vec3 vNpcPos;
	m_pTransformCom->Get_Info(INFO_POS, &vNpcPos);

	vNpcPos.y += 1.f;
	m_pQuestion->Get_TransformCom()->Set_Pos(&vNpcPos);

	if (m_bQuestAccept) // 퀘스트를 받을 수 있는 상태면 (수락 전)
		m_pQuestion->Update_Object(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Cow::LateUpdate_Object(void)
{

	//CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	//CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	//CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	//CGameObject* pNPCSheep = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Sheep");

	//bool bCol = dynamic_cast<CNpcSheep*>(pNPCSheep)->Get_Collision();

	//if (m_bCollision)
	//{
	//	if (GetAsyncKeyState('Z') & 0x8000)
	//	{
	//		dynamic_cast<CNpcText*>(pUIText)->Set_Type(TEXTTYPE::COW);
	//		dynamic_cast<CNpcTextBox*>(pUIBox)->Set_Shown(true);
	//		dynamic_cast<CNpcText*>(pUIText)->Set_Shown(true);
	//	}
	//}
	//else if (!m_bCollision && !bCol) // Cow와 Player가 일정 거리 떨어져있고, Sheep과도 일정 거리 떨어져 있다면
	//{
	//	dynamic_cast<CNpcTextBox*>(pUIBox)->Set_Shown(false);
	//	dynamic_cast<CNpcText*>(pUIText)->Set_Shown(false);
	//}

	if (m_bQuestAccept)
		m_pQuestion->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CNpc_Cow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	if (m_bQuestAccept)
		m_pQuestion->Render_Object();

	__super::Render_Object();
}

CNpc_Cow* CNpc_Cow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Cow* pInstance = new CNpc_Cow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Cow Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Cow::Free()
{
}
