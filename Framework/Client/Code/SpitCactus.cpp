#include "SpitCactus.h"
#include "Export_Function.h"
#include "CactusNeedle.h"
CSpitCactus::CSpitCactus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::SPIT_CACTUS)
{
}

CSpitCactus::CSpitCactus(const CSpitCactus& rhs)
	: CMonster(rhs)
{
}

CSpitCactus::~CSpitCactus()
{
}




HRESULT CSpitCactus::Ready_Object(void)
{
	Set_State(MONSTER_STATE::REGEN);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SpitCactus_Idle_Down", L"Proto_Texture_SpitCactus_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SpitCactus_Attack_Down", L"Proto_Texture_SpitCactus_Attack_Down", 0.1f);
	m_pAnimator->Add_Animation(L"SpitCactus_Regen_Down", L"Proto_Texture_SpitCactus_Regen_Down", 0.1f);
	m_fMinHeight = 1.0f;
	m_pTransformCom->Set_Pos(&_vec3(1.0f, 1.0f, 9.0f));
	Set_Speed(5.f);
	m_pAnimator->Play_Animation(L"SpitCactus_Regen_Down", true);
	m_tStat = { 3,3,1 };
	return S_OK;
}

_int CSpitCactus::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);

	return iExit;
}
void CSpitCactus::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CSpitCactus::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}


void CSpitCactus::Update_Idle(_float fTimeDelta)
{
	if (rand() % 10 > 8)
	{
		Set_State(MONSTER_STATE::ATTACK);
		m_pAnimator->Play_Animation(L"SpitCactus_Attack_Down", false);
	}
}
void CSpitCactus::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SpitCactus_Idle_Down", true);
	}
}


void CSpitCactus::Update_Attack(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3&&m_bShoot)
	{
		for (_int i = 0; i < 8; i++)
		{
			CCactusNeedle* pCactusNeedle = CCactusNeedle::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pCactusNeedle, );
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			pCactusNeedle->Get_TransformCom()->Set_Pos(&vPos);
			_vec3 AxisY = { 0,1,0 };
			pCactusNeedle->Get_TransformCom()->RotationAxis(AxisY, D3DXToRadian(45.f*(float)i));
			pCactusNeedle->Set_Poison(m_bPoison);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
			pLayer->Add_GameObject(L"CactusNeedle", pCactusNeedle);
			m_bShoot = false;
		}
	}
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"SpitCactus_Idle_Down", true);
		m_bShoot = true;
	}
}

void CSpitCactus::Update_Move(_float fTimeDelta)
{
}
void CSpitCactus::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}


HRESULT CSpitCactus::Add_Component(void)
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
CSpitCactus* CSpitCactus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpitCactus* pInstance = new CSpitCactus(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SpitCactus Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSpitCactus::Trace(_float fTimeDelta)
{
}

void CSpitCactus::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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
		m_tStat.iHp -= 1;
		if (m_tStat.iHp < 1)
			Set_State(MONSTER_STATE::DIE);
	}
}