
#include "SilkWorm.h"
#include "BugBall.h"
#include "Export_Function.h"

CSilkWorm::CSilkWorm(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev)
{
}

CSilkWorm::CSilkWorm(const CSilkWorm& rhs) : CMonster(rhs)
{
}

CSilkWorm::~CSilkWorm()
{
}



HRESULT CSilkWorm::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_Down", L"Proto_Texture_SilkWorm_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_Up", L"Proto_Texture_SilkWorm_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_Left", L"Proto_Texture_SilkWorm_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_Right", L"Proto_Texture_SilkWorm_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_RightDown", L"Proto_Texture_SilkWorm_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_RightUp", L"Proto_Texture_SilkWorm_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_LeftDown", L"Proto_Texture_SilkWorm_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Idle_LeftUp", L"Proto_Texture_SilkWorm_Move_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"SilkWorm_Move_Down", L"Proto_Texture_SilkWorm_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_Up", L"Proto_Texture_SilkWorm_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_Left", L"Proto_Texture_SilkWorm_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_Right", L"Proto_Texture_SilkWorm_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_RightDown", L"Proto_Texture_SilkWorm_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_RightUp", L"Proto_Texture_SilkWorm_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_LeftDown", L"Proto_Texture_SilkWorm_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Move_LeftUp", L"Proto_Texture_SilkWorm_Move_LeftUp", 0.1f);

	m_pAnimator->Add_Animation(L"SilkWorm_Attack_Down", L"Proto_Texture_SilkWorm_Attack_Down", 0.4f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_Up", L"Proto_Texture_SilkWorm_Attack_Up", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_Left", L"Proto_Texture_SilkWorm_Attack_Left", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_Right", L"Proto_Texture_SilkWorm_Attack_Right", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_RightDown", L"Proto_Texture_SilkWorm_Attack_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_RightUp", L"Proto_Texture_SilkWorm_Attack_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_LeftDown", L"Proto_Texture_SilkWorm_Attack_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"SilkWorm_Attack_LeftUp", L"Proto_Texture_SilkWorm_Attack_LeftUp", 0.1f);


	m_pTransformCom->Set_Info(INFO_POS, &_vec3(4.0f, 1.0f, 4.0f));
	Set_Speed(5.f);
	Set_State(MONSTER_STATE::IDLE);
	m_pAnimator->Play_Animation(L"SilkWorm_Idle_Down", true);
	m_tStat = { 3,3,1 };

	return S_OK;
}

_int CSilkWorm::Update_Object(const _float& fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);
	_vec3  vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	return iExit;
}
void CSilkWorm::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
	
}
void CSilkWorm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}


void CSilkWorm::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"SilkWorm_Move_Down", true);
		}

		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSilkWorm::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}

void CSilkWorm::Update_Regen(_float fTimeDelta)
{
}

void CSilkWorm::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"SilkWorm_Idle_Down", true);
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

void CSilkWorm::Update_Attack(_float fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);
	Trace(fTimeDelta);

}
HRESULT CSilkWorm::Add_Component(void)
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

CSilkWorm* CSilkWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSilkWorm* pInstance = new CSilkWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SilkWorm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSilkWorm::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
	NULL_CHECK_RETURN(pTarget, );

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = vTargetPos - vPos;
	vDir.y = 0.f;

	if (D3DXVec3Length(&vDir) > 7.f && !m_bShoot)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SilkWorm_Idle_Down", true);
		m_bShoot = false;

	}

	else if (D3DXVec3Length(&vDir) <4.f && !m_bShoot)
	{
		m_pAnimator->Play_Animation(L"SilkWorm_Attack_Down", true);

		m_bShoot = true;

	}
	else if (D3DXVec3Length(&vDir) < 4.f|| m_bShoot)
	{
	
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3&&m_bShoot)
		{
			D3DXVec3Normalize(&vDir, &vDir);
			CBugBall* pBugBall = CBugBall::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pBugBall, );
			_vec3 BulletPos= vPos;
			BulletPos.y += 0.5f;
			BulletPos.z -= 0.01f;
			pBugBall->Get_TransformCom()->Set_Pos(&BulletPos);
			pBugBall->Set_Dir(vDir);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
			pLayer->Add_GameObject(L"BugBall", pBugBall);
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"SilkWorm_Idle_Down", true);

			m_bShoot = false;
		}

	}
	else {
		m_pAnimator->Play_Animation(L"SilkWorm_Move_Down", true);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	}
}
void CSilkWorm::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
		if (Get_State() == MONSTER_STATE::DIE)
		return;
		

		if(_eCollisionGroup==COLLISION_GROUP::COLLIDE_SWING  &&pCollider->GetOwner()->GetObj_Type()==OBJ_TYPE::OBJ_PLAYER)
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

			m_pRigidBodyCom ->AddForce(vDir * 80.0f);
			if (m_tStat.iHp < 1.f)
				Set_State(MONSTER_STATE::DIE);
		}
}