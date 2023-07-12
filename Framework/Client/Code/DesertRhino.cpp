
#include "DesertRhino.h"

#include "Export_Function.h"

CDesertRhino::CDesertRhino(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CMonster(pGraphicDev, OBJ_ID::DESERT_RHINO)
	, m_fMoveTime(0.f)
{
}

CDesertRhino::CDesertRhino(const CDesertRhino& rhs) 
	: CMonster(rhs)
	, m_fMoveTime(0.f)
{
}

CDesertRhino::~CDesertRhino()
{
}


HRESULT CDesertRhino::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_Down", L"Proto_Texture_DesertRhino_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_Up", L"Proto_Texture_DesertRhino_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_Left", L"Proto_Texture_DesertRhino_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_Right", L"Proto_Texture_DesertRhino_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_RightDown", L"Proto_Texture_DesertRhino_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_RightUp", L"Proto_Texture_DesertRhino_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_LeftDown", L"Proto_Texture_DesertRhino_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Idle_LeftUp", L"Proto_Texture_DesertRhino_Move_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_Down", L"Proto_Texture_DesertRhino_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_Up", L"Proto_Texture_DesertRhino_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_Left", L"Proto_Texture_DesertRhino_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_Right", L"Proto_Texture_DesertRhino_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_RightDown", L"Proto_Texture_DesertRhino_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_RightUp", L"Proto_Texture_DesertRhino_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_LeftDown", L"Proto_Texture_DesertRhino_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Move_LeftUp", L"Proto_Texture_DesertRhino_Move_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"DesertRhino_Ready_Down", L"Proto_Texture_DesertRhino_Ready_Down", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_Up", L"Proto_Texture_DesertRhino_Ready_Up", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_Left", L"Proto_Texture_DesertRhino_Ready_Left", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_Right", L"Proto_Texture_DesertRhino_Ready_Right", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_RightDown", L"Proto_Texture_DesertRhino_Ready_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_RightUp", L"Proto_Texture_DesertRhino_Ready_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_LeftDown", L"Proto_Texture_DesertRhino_Ready_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Ready_LeftUp", L"Proto_Texture_DesertRhino_Ready_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"DesertRhino_Attack_Down", L"Proto_Texture_DesertRhino_Attack_Down", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_Up", L"Proto_Texture_DesertRhino_Attack_Up", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_Left", L"Proto_Texture_DesertRhino_Attack_Left", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_Right", L"Proto_Texture_DesertRhino_Attack_Right", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_RightDown", L"Proto_Texture_DesertRhino_Attack_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_RightUp", L"Proto_Texture_DesertRhino_Attack_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_LeftDown", L"Proto_Texture_DesertRhino_Attack_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"DesertRhino_Attack_LeftUp", L"Proto_Texture_DesertRhino_Attack_LeftUp", 0.1f);

	m_pTransformCom->Set_Info(INFO_POS, &_vec3(10.0f, 1.0f, 10.0f));
	Set_Speed(5.f);
	Set_State(MONSTER_STATE::IDLE);
	m_pAnimator->Play_Animation(L"DesertRhino_Idle_Down", true);
	m_fMinHeight = 1.0f;

	return S_OK;
}

_int CDesertRhino::Update_Object(const _float& fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);
	if (Get_State() != MONSTER_STATE::REGEN && Get_State() != MONSTER_STATE::ATTACK)
	{
		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
		NULL_CHECK_RETURN(pTarget, S_OK);

		Set_Target(pTarget);
		_vec3 vTargetPos, vPos, vDir;

		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) < 5.f)
		{
			Set_State(MONSTER_STATE::REGEN);
			m_pAnimator->Play_Animation(L"DesertRhino_Ready_Down", true);
		}
	}


	return iExit;
}

void CDesertRhino::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}
void CDesertRhino::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

void CDesertRhino::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"DesertRhino_Move_Down", true);
		}

		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CDesertRhino::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CDesertRhino::Update_Regen(_float fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);
	_vec3 vTargetPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = vTargetPos - vPos;
	m_vDir = vTargetPos - vPos;

	if (m_fMoveTime > 15.f)
	{
		Set_State(MONSTER_STATE::ATTACK);
		m_pAnimator->Play_Animation(L"DesertRhino_Attack_Down", true);
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;

}

void CDesertRhino::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Down", true);
		}

		vDst = { float(rand() % 10) - 5.f,0.f,float(rand() % 10) - 5.f };
		if (vDst != m_vDst)
			m_vDst = vDst;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
	m_tStat = { 3,3,1 };
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
}

void CDesertRhino::Update_Attack(_float fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);
	Trace(fTimeDelta);
}

HRESULT CDesertRhino::Add_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;
}

CDesertRhino* CDesertRhino::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDesertRhino* pInstance = new CDesertRhino(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("DesertRhino Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDesertRhino::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	vDir = m_vDir;
	vDir = m_vDir;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 2 * Get_Speed());

	if (m_fMoveTime > 10.f)
	{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Down", true);
			m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}
void CDesertRhino::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;


	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vPos - vTargetPos;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pRigidBodyCom->AddForce(vDir * 80.0f);
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1.f)
			Set_State(MONSTER_STATE::DIE);

	}
}