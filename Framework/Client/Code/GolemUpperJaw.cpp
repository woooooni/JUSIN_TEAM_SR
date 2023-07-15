#include "Export_Function.h"
#include "GolemUpperJaw.h"
#include "SunGollem.h"
CGolemUpperJaw::CGolemUpperJaw(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemUpperJaw::CGolemUpperJaw(const CGolemUpperJaw& rhs)
	: CGolemPart(rhs)

{

}

CGolemUpperJaw::~CGolemUpperJaw()
{
}

HRESULT CGolemUpperJaw::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_UpperJaw", L"Proto_Texture_SunGolem_Idle_UpperJaw", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_UpperJaw", L"Proto_Texture_SunGolem_Dirty_UpperJaw", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_UpperJaw", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f,0.6f,0.6f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemUpperJaw::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (Get_State())
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemUpperJaw::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemUpperJaw::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CGolemUpperJaw::Add_Component(void)
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
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}

void CGolemUpperJaw::Update_Idle(_float fTimeDelta)
{
}

void CGolemUpperJaw::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_UpperJaw", true);

}

void CGolemUpperJaw::Update_Move(_float fTimeDelta)
{
}

void CGolemUpperJaw::Update_Attack(_float fTimeDelta)
{
}

void CGolemUpperJaw::Update_Die(_float fTimeDelta)
{
}

void CGolemUpperJaw::Update_Regen(_float fTimeDelta)
{

}
CGolemUpperJaw* CGolemUpperJaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemUpperJaw* pInstance = new CGolemUpperJaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemUpperJaw Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemUpperJaw::Free()
{
	__super::Free();
}
