#include "TrashBig.h"
#include "Export_Function.h"

CTrashBig::CTrashBig(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev, OBJ_ID::TRASH_BIG), m_fMoveTime(0.f)
{
}

CTrashBig::CTrashBig(const CTrashBig& rhs) : CMonster(rhs), m_fMoveTime(0.f)
{
}

CTrashBig::~CTrashBig()
{
}
HRESULT CTrashBig::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_Down", L"Proto_Texture_TrashBig_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_Up", L"Proto_Texture_TrashBig_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_Left", L"Proto_Texture_TrashBig_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_Right", L"Proto_Texture_TrashBig_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_RightDown", L"Proto_Texture_TrashBig_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_RightUp", L"Proto_Texture_TrashBig_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_LeftDown", L"Proto_Texture_TrashBig_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Idle_LeftUp", L"Proto_Texture_TrashBig_Idle_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_Down", L"Proto_Texture_TrashBig_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_Up", L"Proto_Texture_TrashBig_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_Left", L"Proto_Texture_TrashBig_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_Right", L"Proto_Texture_TrashBig_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_RightDown", L"Proto_Texture_TrashBig_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_RightUp", L"Proto_Texture_TrashBig_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_LeftDown", L"Proto_Texture_TrashBig_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBig_Move_LeftUp", L"Proto_Texture_TrashBig_Move_LeftUp", 0.1f);

	m_pTransformCom->Set_Pos(&_vec3(10.0f, 1.0f, 10.0f));
	Set_Speed(5.f);
	Set_State(MONSTER_STATE::IDLE);
	m_pAnimator->Play_Animation(L"TrashBig_Move_Down", true);
	m_tStat = { 3,3,1 };
	return S_OK;
}


_int CTrashBig::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);
	if (MONSTER_STATE::ATTACK != Get_State())
	{
		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
		NULL_CHECK_RETURN(pTarget, S_OK );
		Set_Target(pTarget);
		_vec3 vTargetPos, vPos, vDir;
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vTargetPos - vPos;
		if (D3DXVec3Length(&vDir) < 5.f)
		{
			Set_State(MONSTER_STATE::ATTACK);
			m_pAnimator->Play_Animation(L"TrashBig_Move_Down", true);
		}
	}	

	return iExit;
}

void CTrashBig::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}


void CTrashBig::LateUpdate_Object(void)
{
}

void CTrashBig::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)



		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashBig_Move_Down", true);
		}
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashBig::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CTrashBig::Update_Regen(_float fTimeDelta)
{
}

void CTrashBig::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_pAnimator->GetCurrAnimation()->Is_Finished() && m_pRigidBodyCom->IsGround())
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashBig_Idle_Down", true);
		}
		vDst = { float(rand() % 10) - 5.f,0.f,float(rand() % 10) - 5.f };
		if (vDst != m_vDst)
			m_vDst = vDst;
		m_bJump = false;
	}
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	if (!m_bJump && m_pAnimator->GetCurrAnimation()->Get_Idx() == 2)
	{
		m_pRigidBodyCom->AddForce(vDir * 40.0f);
		m_pRigidBodyCom->AddForce(_vec3(0.0f, 80.0f, 0.0f));
		m_pRigidBodyCom->SetGround(false);
		m_bJump = true;
	}

	if (m_pRigidBodyCom->GetVelocity().y > 0.0f && m_pAnimator->GetCurrAnimation()->Get_Idx() > 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}
	if (m_pRigidBodyCom->GetVelocity().y < 0.0f && m_pAnimator->GetCurrAnimation()->Get_Idx() < 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}


	m_pTarget = nullptr;
}

void CTrashBig::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}


HRESULT CTrashBig::Add_Component(void)
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

CTrashBig* CTrashBig::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashBig* pInstance = new CTrashBig(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BlueBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrashBig::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.f;
	if (m_pAnimator->GetCurrAnimation()->Is_Finished() && m_pRigidBodyCom->IsGround())
	{
		if (D3DXVec3Length(&vDir) > 5.f)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashBig_Idle_Down", true);
			return;
		}
		m_bJump = false;
	}
	if (D3DXVec3Length(&vDir) > 5.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"TrashBig_Idle_Down", true);
		return;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	if (!m_bJump && m_pAnimator->GetCurrAnimation()->Get_Idx() == 2)
	{
		m_pRigidBodyCom->AddForce(vDir * 40.0f);
		m_pRigidBodyCom->AddForce(_vec3(0.0f, 80.0f, 0.0f));
		m_pRigidBodyCom->SetGround(false);
		m_bJump = true;
	}

	if (m_pRigidBodyCom->GetVelocity().y > 0.0f && m_pAnimator->GetCurrAnimation()->Get_Idx() > 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}
	if (m_pRigidBodyCom->GetVelocity().y < 0.0f && m_pAnimator->GetCurrAnimation()->Get_Idx() < 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}
}

void CTrashBig::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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
		if (m_tStat.iHp < 1)
			Set_State(MONSTER_STATE::DIE);
	}
}