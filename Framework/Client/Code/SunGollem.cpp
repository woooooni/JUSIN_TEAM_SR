#include "Export_Function.h"
#include "SunGollem.h"

CSunGollem::CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_fSpeed(5.f)
{
}
CSunGollem::CSunGollem(const CSunGollem& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_eState(rhs.m_eState)
{

}

CSunGollem::~CSunGollem()
{
}

HRESULT CSunGollem::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Body", L"Proto_Texture_SunGolem_Idle_Head", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Body", L"Proto_Texture_SunGolem_Dirty_Body", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);

	m_pTransformCom->Set_Pos(&_vec3(2.f, 2.f, 2.f));
	m_pTransformCom->Set_Scale({ 2.f, 2.f, 2.f });
	Set_Speed(5.f);
	Set_State(SUNGOLEM_STATE::REGEN);
	
	return S_OK;
}

_int CSunGollem::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (m_eState)
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

void CSunGollem::LateUpdate_Object(void)
{
}

void CSunGollem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSunGollem::Add_Component(void)
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

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}

void CSunGollem::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_fMoveTime > 5.f)
	{
		vDir = { 0.f, 5.f * fTimeDelta, 0.f };
	}
	else
	{
		vDir = { 0.f, -5.f * fTimeDelta, 0.f };
	}
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	if (m_fMoveTime > 10.f)
	{
	
//			Set_State(SUNGOLEM_STATE::DIRTY);
//			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Body", true);
			m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Dirty(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_fMoveTime > 5.f)
	{
		vDir = { 0.f, 5.f * fTimeDelta, 0.f };
	}
	else
	{
		vDir = { 0.f, -5.f * fTimeDelta, 0.f };
	}
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	vDir = { 0.f, 1.f * fTimeDelta, 0.f };
	if (m_fMoveTime > 10.f)
	{

		Set_State(SUNGOLEM_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Move(_float fTimeDelta)
{
}

void CSunGollem::Update_Attack(_float fTimeDelta)
{
}

void CSunGollem::Update_Die(_float fTimeDelta)
{
}

void CSunGollem::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0.f, 1.f * fTimeDelta, 0.f };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y > 2.f)
	{
		vPos.y = 2.f;
		m_pTransformCom->Set_Info(INFO_POS, &vPos);
		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CSunGollem* CSunGollem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSunGollem* pInstance = new CSunGollem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SunGollem Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSunGollem::Free()
{
	__super::Free();
}
