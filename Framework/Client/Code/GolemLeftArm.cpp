#include "Export_Function.h"
#include "GolemLeftArm.h"
#include "SunGollem.h"

CGolemLeftArm::CGolemLeftArm(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
, m_eState(SUNGOLEM_STATE::REGEN)

{
}
CGolemLeftArm::CGolemLeftArm(const CGolemLeftArm& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(rhs.m_eState)
{

}

CGolemLeftArm::~CGolemLeftArm()
{
}

HRESULT CGolemLeftArm::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_LeftArm", L"Proto_Texture_SunGolem_Idle_LeftArm", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_LeftArm", L"Proto_Texture_SunGolem_Dirty_LeftArm", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LeftArm", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.7f, 0.7f,0.7f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;

}

_int CGolemLeftArm::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"SunGollem");
	Set_State(dynamic_cast<CSunGollem*>(pTarget)->Get_State());
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

void CGolemLeftArm::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemLeftArm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGolemLeftArm::Add_Component(void)
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

void CGolemLeftArm::Update_Idle(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LeftArm", true);
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);
	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
			m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;


	


}

void CGolemLeftArm::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_LeftArm", true);
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);
	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10 * fTimeDelta;
}

void CGolemLeftArm::Update_Move(_float fTimeDelta)
{
}

void CGolemLeftArm::Update_Attack(_float fTimeDelta)
{
}

void CGolemLeftArm::Update_Die(_float fTimeDelta)
{
}

void CGolemLeftArm::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0,1 ,0 };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fTimeDelta);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y > 2.f)
	{

		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CGolemLeftArm* CGolemLeftArm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemLeftArm* pInstance = new CGolemLeftArm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemLeftArm Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemLeftArm::Free()
{
	__super::Free();
}
