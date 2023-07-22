#include "Export_Function.h"
#include "Npc.h"

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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_NPC_Sheep_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	// NPC TEST
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep", L"Proto_Texture_NPC_Sheep_Idle", 0.1f), E_FAIL);
	//FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"NPC_Tutorial_Sheep", L"Proto_Texture_NPC_Sheep_React", 0.1f), E_FAIL);

	// IDle , React 2가지 상태있음. 설정 추가필요
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"NPC_Tutorial_Sheep", TRUE), E_FAIL);

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
