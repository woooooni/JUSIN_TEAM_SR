#include "../Include/stdafx.h"
#include "Npc_Sheep.h"
#include "Export_Function.h"

CNpc_Sheep::CNpc_Sheep(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev)
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
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_Idle", L"Proto_Texture_NPC_Sheep_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_React", L"Proto_Texture_NPC_Sheep_React", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Sheep_Idle", TRUE), E_FAIL);

	m_pExclamation = CUI_ExclamationMark::Create(m_pGraphicDev);
	if (m_pExclamation != nullptr)
		m_pExclamation->Set_Owner(this);

	return S_OK;
}

_int CNpc_Sheep::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	//CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	//CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	//CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	//CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	//CGameObject* pNPCCow = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Cow");

	//_vec3 vPlayerPos, vNPCPos;
	//bool bShown = dynamic_cast<CNpcTextBox*>(pUIBox)->Get_Shown();
	//bool bCol = dynamic_cast<CNpc_Cow*>(pNPCCow)->Get_Collision();

	//pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	//m_pTransformCom->Get_Info(INFO_POS, &vNPCPos);
	//_float fLength = D3DXVec3Length(&(vPlayerPos - vNPCPos));

	//if (fLength <= 2.f && !bCol)
	//{
	//	bool bShown = dynamic_cast<CNpcTextBox*>(pUIBox)->Get_Shown();

	//	m_bCollision = true;
	//	dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(true);

	//	if (bShown)
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
	//m_pQuestion->Get_TransformCom()->Set_Pos(&vNpcPos);
	m_pExclamation->Get_TransformCom()->Set_Pos(&vNpcPos);

	//if (m_bQuestAccept) // 퀘스트를 받을 수 있는 상태면 (수락 전)
		//m_pQuestion->Update_Object(fTimeDelta);
	if (!m_bQuestAccept)
		m_pExclamation->Update_Object(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNpc_Sheep::LateUpdate_Object(void)
{

	//CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	//CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	//CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	//CGameObject* pNPCCow = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Cow");

	//bool bCol = dynamic_cast<CNpc_Cow*>(pNPCCow)->Get_Collision();

	//if (m_bCollision)
	//{
	//	if (GetAsyncKeyState('Z') & 0x8000)
	//	{
	//		dynamic_cast<CNpcText*>(pUIText)->Set_Type(TEXTTYPE::SHEEP);
	//		dynamic_cast<CNpcTextBox*>(pUIBox)->Set_Shown(true);
	//		dynamic_cast<CNpcText*>(pUIText)->Set_Shown(true);
	//	}
	//}
	//else if (!m_bCollision && !bCol)
	//{
	//	dynamic_cast<CNpcTextBox*>(pUIBox)->Set_Shown(false);
	//	dynamic_cast<CNpcText*>(pUIText)->Set_Shown(false);
	// }

	if (!m_bQuestAccept)
		m_pExclamation->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CNpc_Sheep::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	if (!m_bQuestAccept)
		m_pExclamation->Render_Object();

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
}

