#include "../Include/stdafx.h"
#include "NPCSheep.h"
#include "Export_Function.h"

CNPCSheep::CNPCSheep(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNPC(pGraphicDev)
{
}

CNPCSheep::CNPCSheep(const CNPCSheep& rhs)
	: CNPC(rhs)
{
}

CNPCSheep::~CNPCSheep()
{
}

HRESULT CNPCSheep::Ready_Object(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_NPC_Sheep_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_Idle", L"Proto_Texture_NPC_Sheep_Idle", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep_React", L"Proto_Texture_NPC_Sheep_React", 0.5f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Sheep_Idle", TRUE), E_FAIL);


	return S_OK;
}

_int CNPCSheep::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	CGameObject* pPlayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	CGameObject* pNPCCow = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Cow");

	_vec3 vPlayerPos, vNPCPos;
	bool bShown = dynamic_cast<CNPCTextBox*>(pUIBox)->Get_Shown();
	bool bCol = dynamic_cast<CNPCCow*>(pNPCCow)->Get_Collision();

	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vNPCPos);
	_float fLength = D3DXVec3Length(&(vPlayerPos - vNPCPos));

	if (fLength <= 2.f && !bCol)
	{
		bool bShown = dynamic_cast<CNPCTextBox*>(pUIBox)->Get_Shown();

		m_bCollision = true;
		dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(true);

		if (bShown)
			dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	}

	else if (fLength > 2.f)
	{
		m_bCollision = false;

		if (!bCol)
			dynamic_cast<CUI_ShortCutKey*>(pUI)->Set_Shown(false);
	}

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CNPCSheep::LateUpdate_Object(void)
{

	CGameObject* pUI = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"UI_ShortCutKey_Info");
	CGameObject* pUIBox = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_TextBox");
	CGameObject* pUIText = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::UI)->Find_GameObject(L"NPC_Text");
	CGameObject* pNPCCow = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"NPC_Tutorial_Cow");

	bool bCol = dynamic_cast<CNPCCow*>(pNPCCow)->Get_Collision();

	if (m_bCollision)
	{
		if (GetAsyncKeyState('Z') & 0x8000)
		{
			dynamic_cast<CNPCText*>(pUIText)->Set_Type(TEXTTYPE::SHEEP);
			dynamic_cast<CNPCTextBox*>(pUIBox)->Set_Shown(true);
			dynamic_cast<CNPCText*>(pUIText)->Set_Shown(true);
		}
	}
	else if (!m_bCollision && !bCol)
	{
		dynamic_cast<CNPCTextBox*>(pUIBox)->Set_Shown(false);
		dynamic_cast<CNPCText*>(pUIText)->Set_Shown(false);
	}

	__super::LateUpdate_Object();
}

void CNPCSheep::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();
}

CNPCSheep* CNPCSheep::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, NPCTYPE eType)
{
	CNPCSheep* pInstance = new CNPCSheep(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Sheep Create Failed");
		return nullptr;
	}

	pInstance->Get_TransformCom()->Set_Pos(&vPos);

	return pInstance;
}

void CNPCSheep::Free()
{
}

