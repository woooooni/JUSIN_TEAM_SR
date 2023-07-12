#include "Export_Function.h"
#include "GolemRightHand.h"
#include "SunGollem.h"

CGolemRightHand::CGolemRightHand(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SUN_GOLLEM)
, m_eState(SUNGOLEM_STATE::REGEN)

{
}
CGolemRightHand::CGolemRightHand(const CGolemRightHand& rhs)
	: Engine::CGameObject(rhs)
	, m_eState(rhs.m_eState)
{

}

CGolemRightHand::~CGolemRightHand()
{
}

HRESULT CGolemRightHand::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"SunGolem_Idle_RightHand", L"Proto_Texture_SunGolem_Idle_RightHand", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_RightHand", L"Proto_Texture_SunGolem_Dirty_RightHand", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerRightHand", L"Proto_Texture_SunGolem_Dirty_BummerRightHand", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_RightHand", true);

	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.7f, 0.7f,0.7f });

	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemRightHand::Update_Object(const _float& fTimeDelta)
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

void CGolemRightHand::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemRightHand::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CGolemRightHand::Add_Component(void)
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

void CGolemRightHand::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.f ,1.f ,0.f };
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
	m_fMoveTime += 10.f * fTimeDelta;


	


}

void CGolemRightHand::Update_Dirty(_float fTimeDelta)
{
	if (m_iArmNum != 1)
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_RightHand", true);
	else
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerRightHand", true);
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.f ,1.f ,0.f };
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
	m_fMoveTime += 10.f * fTimeDelta;
}

void CGolemRightHand::Update_Move(_float fTimeDelta)
{
}

void CGolemRightHand::Update_Attack(_float fTimeDelta)
{
}

void CGolemRightHand::Update_Die(_float fTimeDelta)
{
}

void CGolemRightHand::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0.f,1.f ,0.f };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fTimeDelta);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y > 2.f)
	{

		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CGolemRightHand* CGolemRightHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemRightHand* pInstance = new CGolemRightHand(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemRightHand Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemRightHand::Free()
{
	__super::Free();
}
