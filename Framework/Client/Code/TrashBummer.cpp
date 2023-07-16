
#include "TrashBummer.h"
#include "SludgeBall.h"
#include "Export_Function.h"

CTrashBummer::CTrashBummer(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev, OBJ_ID::MORTH_MAGE)
{
}

CTrashBummer::CTrashBummer(const CTrashBummer& rhs) : CMonster(rhs)
{
}

CTrashBummer::~CTrashBummer()
{
}



HRESULT CTrashBummer::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Down", L"Proto_Texture_TrashBummer_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Up", L"Proto_Texture_TrashBummer_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Left", L"Proto_Texture_TrashBummer_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_Right", L"Proto_Texture_TrashBummer_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_RightDown", L"Proto_Texture_TrashBummer_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_RightUp", L"Proto_Texture_TrashBummer_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_LeftDown", L"Proto_Texture_TrashBummer_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Idle_LeftUp", L"Proto_Texture_TrashBummer_Move_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Down", L"Proto_Texture_TrashBummer_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Up", L"Proto_Texture_TrashBummer_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Left", L"Proto_Texture_TrashBummer_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_Right", L"Proto_Texture_TrashBummer_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_RightDown", L"Proto_Texture_TrashBummer_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_RightUp", L"Proto_Texture_TrashBummer_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_LeftDown", L"Proto_Texture_TrashBummer_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Move_LeftUp", L"Proto_Texture_TrashBummer_Move_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Down", L"Proto_Texture_TrashBummer_Attack_Down", 0.3f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Up", L"Proto_Texture_TrashBummer_Attack_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Left", L"Proto_Texture_TrashBummer_Attack_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_Right", L"Proto_Texture_TrashBummer_Attack_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_RightDown", L"Proto_Texture_TrashBummer_Attack_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_RightUp", L"Proto_Texture_TrashBummer_Attack_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_LeftDown", L"Proto_Texture_TrashBummer_Attack_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashBummer_Attack_LeftUp", L"Proto_Texture_TrashBummer_Attack_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"TrashBummer_Regen_Down", L"Proto_Texture_TrashBummer_Regen_Down", 0.4f);

	m_pTransformCom->Set_Info(INFO_POS, &_vec3(4.0f, 1.0f, 4.0f));
	Set_Speed(5.f);
	Set_State(MONSTER_STATE::IDLE);
	m_pAnimator->Play_Animation(L"TrashBummer_Regen_Down", true);
	m_tStat = { 3,3,1 };
	m_fMinHeight = 1.0f;
	return S_OK;
}

_int CTrashBummer::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	_int iExit = __super::Update_Object(fTimeDelta);
	_vec3  vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (Get_State() != MONSTER_STATE::REGEN && Get_State() != MONSTER_STATE::ATTACK)
	{
		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
		if (nullptr == pTarget)
			return S_OK;
		Set_Target(pTarget);
		_vec3 vTargetPos,  vDir;

		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);


		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) <= 15.f)
		{
			Set_State(MONSTER_STATE::ATTACK);
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);

		}
	}

	return iExit;
}
void CTrashBummer::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;
	__super::LateUpdate_Object();

}
void CTrashBummer::Render_Object(void)
{
	if (!Is_Active())
		return ;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	

}


void CTrashBummer::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);
		}

		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashBummer::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CTrashBummer::Update_Regen(_float fTimeDelta)
{
}

void CTrashBummer::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);
		}

		vDst = { float(rand() % 10) - 5.f,0.f,float(rand() % 10) - 5.f };
		if (vDst != m_vDst)
			m_vDst = vDst;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
}

void CTrashBummer::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);

}
HRESULT CTrashBummer::Add_Component(void)
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

CTrashBummer* CTrashBummer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashBummer* pInstance = new CTrashBummer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TrashBummer Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrashBummer::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	if (nullptr == pTarget)
		return;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = vTargetPos - vPos;
	vDir.y = 0.f;

	if (D3DXVec3Length(&vDir) > 15.f && !m_bShoot)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);
		m_bShoot = false;

	}

	else if (D3DXVec3Length(&vDir) <9.f && !m_bShoot)
	{
		m_pAnimator->Play_Animation(L"TrashBummer_Attack_Down", true);

		m_bShoot = true;

	}
	else if (D3DXVec3Length(&vDir) < 9.f|| m_bShoot)
	{
	
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3&&m_bShoot)
		{
			D3DXVec3Normalize(&vDir, &vDir);
			CSludgeBall* pSludgeBall = CSludgeBall::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pSludgeBall, );
			_vec3 BulletPos= vPos;
			BulletPos.y += 0.5f;
			BulletPos.z -= 0.01f;
			pSludgeBall->Get_TransformCom()->Set_Pos(&BulletPos);
			pSludgeBall->Set_Dst(vTargetPos);
			pSludgeBall->Set_Owner(this);
			pSludgeBall->Set_Atk(m_tStat.iAttack);
			pSludgeBall->Get_RigidBodyCom()->SetMass(10.f);
			pSludgeBall->Get_RigidBodyCom()->AddForce(_vec3(0.0f, 150.0f, 0.0f));
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
			pLayer->Add_GameObject(L"SludgeBall", pSludgeBall);
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashBummer_Idle_Down", true);

			m_bShoot = false;
		}

	}
	else {
		m_pAnimator->Play_Animation(L"TrashBummer_Move_Down", true);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	}
}
void CTrashBummer::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;

	__super::Collision_Enter(pCollider, _eCollisionGroup, _iColliderID);

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		m_tStat.iHp -= 1;
		_vec3 vTargetPos;
		_vec3 vPos;
		_vec3 vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vPos - vTargetPos;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pRigidBodyCom->AddForce(vDir * 80.0f);
		if (m_tStat.iHp < 1.f)
			Set_State(MONSTER_STATE::DIE);
	}
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER && pCollider->GetOwner()->GetObj_Id() == OBJ_ID::CLEAR_FIELD)
	{
		Set_State(MONSTER_STATE::DIE);
	}
	
}
