#include "BlueBeatle.h"
#include "Export_Function.h"

CBlueBeatle::CBlueBeatle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev), m_fMoveTime(0.f)
{
}

CBlueBeatle::CBlueBeatle(const CBlueBeatle& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CBlueBeatle::~CBlueBeatle()
{
}


HRESULT CBlueBeatle::Ready_Object(void)
{
	Set_State(MONSTER_STATE::IDLE);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"BlueBeatle_Idle_Down", L"Proto_Texture_BlueBeatle_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"BlueBeatle_Move_Down", L"Proto_Texture_BlueBeatle_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"RedBeatle_Idle_Down", L"Proto_Texture_RedBeatle_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"RedBeatle_Move_Down", L"Proto_Texture_RedBeatle_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"GreenBeatle_Idle_Down", L"Proto_Texture_GreenBeatle_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"GreenBeatle_Move_Down", L"Proto_Texture_GreenBeatle_Move_Down", 0.1f);

	m_pTransformCom->Set_Pos(&_vec3(10.0f, 1.0f, 10.0f));
	Set_Speed(5.f);

	switch (m_tInfo.eType)
	{
	case(BEATLETYPE::REDBEATLE):
		m_pAnimator->Play_Animation(L"RedBeatle_Idle_Down", true);
		break;

	case(BEATLETYPE::BLUEBEATLE):
		m_pAnimator->Play_Animation(L"BlueBeatle_Idle_Down", true);
		break;

	case(BEATLETYPE::GREENBEATLE):
		m_pAnimator->Play_Animation(L"GreenBeatle_Idle_Down", true);
		break;

	default:
		break;
	}
	//	m_pAnimator->Play_Animation(L"BlueBeatle_Idle_Down", true);

	return S_OK;
}


_int CBlueBeatle::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);

	return iExit;
}

void CBlueBeatle::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBlueBeatle::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void CBlueBeatle::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);

			switch (m_tInfo.eType)
			{
			case(BEATLETYPE::REDBEATLE):
				m_pAnimator->Play_Animation(L"RedBeatle_Move_Down", true);
				break;

			case(BEATLETYPE::BLUEBEATLE):
				m_pAnimator->Play_Animation(L"BlueBeatle_Move_Down", true);
				break;

			case(BEATLETYPE::GREENBEATLE):
				m_pAnimator->Play_Animation(L"GreenBeatle_Move_Down", true);
				break;

			default:
				break;
			}
		}

		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CBlueBeatle::Update_Regen(_float fTimeDelta)
{
}


void CBlueBeatle::Update_Attack(_float fTimeDelta)
{
}

void CBlueBeatle::Update_Move(_float fTimeDelta)
{

	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);

			switch (m_tInfo.eType)
			{
			case(BEATLETYPE::REDBEATLE):
				m_pAnimator->Play_Animation(L"RedBeatle_Idle_Down", true);
				break;

			case(BEATLETYPE::BLUEBEATLE):
				m_pAnimator->Play_Animation(L"BlueBeatle_Idle_Down", true);
				break;

			case(BEATLETYPE::GREENBEATLE):
				m_pAnimator->Play_Animation(L"GreenBeatle_Idle_Down", true);
				break;

			default:
				break;
			}
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

void CBlueBeatle::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}


HRESULT CBlueBeatle::Add_Component(void)
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

void CBlueBeatle::Set_Type(BEATLETYPE eType)
{
	m_tInfo.eType = eType;
}

CBlueBeatle* CBlueBeatle::Create(LPDIRECT3DDEVICE9 pGraphicDev, BEATLETYPE eType)
{
	CBlueBeatle* pInstance = new CBlueBeatle(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BlueBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlueBeatle::Trace(_float fTimeDelta)
{
}

void CBlueBeatle::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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
		m_tStat.iHp -= 1.f;
		if (m_tStat.iHp < 1.f)
			Set_State(MONSTER_STATE::DIE);
	}
}