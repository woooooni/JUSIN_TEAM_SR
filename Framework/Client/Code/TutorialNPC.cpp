#include "Export_Function.h"
#include "TutorialNPC.h"

CTutorialNPC::CTutorialNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNPC(pGraphicDev), m_tInfo{}
{
}

CTutorialNPC::CTutorialNPC(const CTutorialNPC& rhs)
	: CNPC(rhs), m_tInfo(rhs.m_tInfo)
{
}

CTutorialNPC::~CTutorialNPC()
{
}

HRESULT CTutorialNPC::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTutorialNPC::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

//	m_tInfo.eState = NPCSTATE::NPC_IDLE;
//	m_tInfo.bCollision = false;

//	m_vecNPC.push_back({ m_tInfo.eType , m_tInfo.eState, m_tInfo.bCollision });

	CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"UI_ShortCutKey_Info");
	CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_TextBox");

	_vec3 vPlayerPos, vNPCPos;
	bool bShown = dynamic_cast<CNPCText*>(pUIBox)->Get_Shown();
	
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vNPCPos);
	_float fLength = D3DXVec3Length(&(vPlayerPos - vNPCPos));

	if (fLength <= 2.5f)
	{
		m_tInfo.bCollision = true;
		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(true);

		if (bShown)
			dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	}
	else if (fLength > 2.5f)
	{
		m_tInfo.bCollision = false;
		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	}

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CTutorialNPC::LateUpdate_Object(void)
{
	// Test
	//Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

	CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"UI_ShortCutKey_Info");
	CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_TextBox");
	CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Text");

	bool bShown = dynamic_cast<CUI_ShortCutKey*>(pUI)->Get_Shown();

	if (m_tInfo.bCollision)
	{
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			dynamic_cast<CNPCText*>(pUIBox)->Set_Shown(true);
			dynamic_cast<CTextBox*>(pUIText)->Set_Shown(true);
			//dynamic_cast<CNPCText*>(pUIText)->

		//	if (bShown)
		//		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
		}
	}
	else
	{
		dynamic_cast<CNPCText*>(pUIBox)->Set_Shown(false);
		dynamic_cast<CTextBox*>(pUIText)->Set_Shown(false);
	}

	__super::LateUpdate_Object();
}

void CTutorialNPC::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

HRESULT CTutorialNPC::Add_Component(void)
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

	switch (m_tInfo.eType)
	{
	case NPCTYPE::TUT_SHEEP:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_NPC_Sheep_Idle"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_Idle", L"Proto_Texture_NPC_Sheep_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_React", L"Proto_Texture_NPC_Sheep_React", 0.5f), E_FAIL);

		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Sheep_Idle", TRUE), E_FAIL);
		break;

	case NPCTYPE::TUT_PIG:
		m_tInfo.eState = NPCSTATE::NPC_IDLE;

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_NPC_Pig_Idle"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Pig_Idle", L"Proto_Texture_NPC_Pig_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Pig_React", L"Proto_Texture_NPC_Pig_React", 0.5f), E_FAIL);

		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Pig_Idle", TRUE), E_FAIL);
		break;

//	case NPCTYPE::TUT_COW:
//		m_tInfo.eState = NPCSTATE::NPC_IDLE;
//
//		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_NPC_Cow_Idle"));
//		NULL_CHECK_RETURN(pComponent, E_FAIL);
//		pComponent->SetOwner(this);
//		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
//
//		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Cow_Idle", L"Proto_Texture_NPC_Cow_Idle", 0.5f), E_FAIL);
//		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Cow_React", L"Proto_Texture_NPC_Cow_React", 0.5f), E_FAIL);
//
//		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Cow_Idle", TRUE), E_FAIL);
//		break;

	case NPCTYPE::TUT_DOOGEE:
		m_tInfo.eState = NPCSTATE::NPC_IDLE;

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_NPC_Cow_Idle"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Doogee_Idle", L"Proto_Texture_NPC_Doogee_Idle", 0.5f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Doogee_Dig", L"Proto_Texture_NPC_Doogee_Dig", 0.3f), E_FAIL);
		FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Doogee_React", L"Proto_Texture_NPC_Doogee_React", 0.5f), E_FAIL);

		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Doogee_Idle", TRUE), E_FAIL);
		break;

	default:
		return E_FAIL;
		break;
	}

	// 대화하기 Z 단축기 안내UI
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_ShortKey"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//pComponent->SetOwner(this);
	//m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CTutorialNPC::Set_Type(NPCTYPE eType)
{
	m_tInfo.eType = eType;
}

void CTutorialNPC::Set_State(NPCSTATE eState)
{
	m_tInfo.eState = eState;
}

CTutorialNPC* CTutorialNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, NPCTYPE eType)
{
	CTutorialNPC* pInstance = new CTutorialNPC(pGraphicDev);

	pInstance->Set_Type(eType);
	

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Tutorial NPC Create Failed");
		return nullptr;
	}
	
	pInstance->Get_TransformCom()->Set_Pos(&vPos);

	return pInstance;
}

void CTutorialNPC::Free()
{
	__super::Free();
}
