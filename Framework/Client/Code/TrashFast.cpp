#include "TrashFast.h"
#include "Export_Function.h"

// TrashBig과 같은 패턴으로 Player공격
// 이동거리가 더 길고 스피드도 빠름

CTrashFast::CTrashFast(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::TRASH_BIG), m_fMoveTime(0.f)
{
}

CTrashFast::CTrashFast(const CTrashFast& rhs)
	: CMonster(rhs), m_fMoveTime(0.f)
{
}

CTrashFast::~CTrashFast()
{
}

HRESULT CTrashFast::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos = { 5.f, 1.f, 5.f };
	_vec3 vScale;

	m_pTransformCom->Set_Pos(&vPos);
	Set_Speed(6.f);

	vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_Scale({ vScale.x * 0.6f, vScale.y * 0.6f, vScale.x });

	m_fMinHeight = 1.0f; // 최소y값
	m_tStat = { 1,1,1 };
	//typedef struct tagMonsterstats
	//{
	//	_uint iMaxHp = 0;
	//	_uint iHp = 0;
	//	_uint iAttack = 0;
	//}MONSTERSTAT;

	m_pAnimator->Play_Animation(L"TrashFast_Idle_Down", TRUE);
	Set_State(MONSTER_STATE::IDLE);

	return S_OK;
}

_int CTrashFast::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);

	if (MONSTER_STATE::ATTACK != Get_State()) // 공격상태가 아니면
	{

		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
		NULL_CHECK_RETURN(pTarget, S_OK);
		Set_Target(pTarget); // Player로 타켓을 설정해둠

		_vec3 vTargetPos, vPos, vDir;
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) < 20.f)
		{
			Set_State(MONSTER_STATE::ATTACK); // Player가 10.f안으로 오면 공격상태로 State 변환
			m_pAnimator->Play_Animation(L"TrashFast_Move_Down", TRUE);
		}

	}
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CTrashFast::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTrashFast::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();

	m_pBufferCom->Render_Buffer();
}

void CTrashFast::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashFast_Move_Down", TRUE);
		}
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashFast::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vDst;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished()
		&& m_pRigidBodyCom->IsGround())
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashFast_Idle_Down", TRUE);
		}
		vDst = { float(rand() % 10) - 5.f, 1.f, float(rand() % 10) - 5.f };

		if (vDst != m_vDst)
			m_vDst = vDst;

		m_bJump = FALSE;
	}

	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	if (!m_bJump
		&& m_pAnimator->GetCurrAnimation()->Get_Idx() == 2)
	{
		// void AddForce(_vec3 _vForce) { m_vForce += _vForce; }
		m_pRigidBodyCom->AddForce(vDir * 80.0f);
		m_pRigidBodyCom->AddForce(_vec3(0.0f, 120.0f, 0.0f));
		m_pRigidBodyCom->SetGround(FALSE);
		m_bJump = TRUE;
	}

	if (m_pRigidBodyCom->GetVelocity().y > 0.0f // 속도
		&& m_pAnimator->GetCurrAnimation()->Get_Idx() > 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}

	if (m_pRigidBodyCom->GetVelocity().y < 0.0f
		&& m_pAnimator->GetCurrAnimation()->Get_Idx() < 2)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}

	m_pTarget = nullptr;
}

void CTrashFast::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}

void CTrashFast::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(FALSE);
}

void CTrashFast::Update_Regen(_float fTimeDelta)
{
}

HRESULT CTrashFast::Add_Component(void)
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	// IDLE Animation
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_Down", L"Proto_Texture_TrashFast_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_Up", L"Proto_Texture_TrashFast_Idle_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_Left", L"Proto_Texture_TrashFast_Idle_Left", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_Right", L"Proto_Texture_TrashFast_Idle_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_RightDown", L"Proto_Texture_TrashFast_Idle_RightDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_RightUp", L"Proto_Texture_TrashFast_Idle_RightUp", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_LeftDown", L"Proto_Texture_TrashFast_Idle_LeftDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Idle_LeftUp", L"Proto_Texture_TrashFast_Idle_LeftUp", 0.1f), E_FAIL);
	// MOVE Animation
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_Down", L"Proto_Texture_TrashFast_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_Up", L"Proto_Texture_TrashFast_Move_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_Left", L"Proto_Texture_TrashFast_Move_Left", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_Right", L"Proto_Texture_TrashFast_Move_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_RightDown", L"Proto_Texture_TrashFast_Move_RightDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_RightUp", L"Proto_Texture_TrashFast_Move_RightUp", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_LeftDown", L"Proto_Texture_TrashFast_Move_LeftDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Move_LeftUp", L"Proto_Texture_TrashFast_Move_LeftUp", 0.1f), E_FAIL);
	// REGEN Animation
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"TrashFast_Regen_Down", L"Proto_Texture_TrashFast_Regen_Down", 0.1f), E_FAIL);

	return S_OK;
}

void CTrashFast::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.f;
	if (m_pAnimator->GetCurrAnimation()->Is_Finished() && m_pRigidBodyCom->IsGround())
	{
		if (D3DXVec3Length(&vDir) > 20.f)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashFast_Idle_Down", TRUE);
			return;
		}
		m_bJump = false;
	}
	if (D3DXVec3Length(&vDir) > 20.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"TrashFast_Idle_Down", TRUE);
		return;
	}

	D3DXVec3Normalize(&vDir, &vDir);

	if (!m_bJump && m_pAnimator->GetCurrAnimation()->Get_Idx() == 2)
	{
		m_pRigidBodyCom->AddForce(vDir * 80.0f); // vDir.y가 0임. y를 AddForce해줘야함.
		m_pRigidBodyCom->AddForce(_vec3(0.0f, 120.0f, 0.0f));
		m_pRigidBodyCom->SetGround(FALSE);
		m_bJump = TRUE;
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

void CTrashFast::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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

CTrashFast* CTrashFast::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashFast* pInstance = new CTrashFast(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TrashFast Create Failed");
		return nullptr;
	}

	return pInstance;
}
