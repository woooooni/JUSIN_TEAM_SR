#include "Cupa.h"
#include "Export_Function.h"

CCupa::CCupa(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::CUPA), m_fMoveTime(0.f)
{
}

CCupa::CCupa(const CCupa& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CCupa::~CCupa()
{
}

HRESULT CCupa::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_State(MONSTER_STATE::IDLE);
	Set_Speed(5.f);

	_vec3 vPos = _vec3(1.f, 1.f, 1.f);
	_vec3 vScale = _vec3(2.f, 2.f, 2.f);

	m_pTransformCom->Set_Pos(&vPos);
	m_pTransformCom->Set_Scale(vScale);


	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_Down", L"Proto_Texture_Cupa_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_Down", L"Proto_Texture_Cupa_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_Down", L"Proto_Texture_Cupa_Attack_Down", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Regen_Down", L"Proto_Texture_Cupa_Regen_Down", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"Cupa_Idle_Down", TRUE), E_FAIL);

	return S_OK;
}

_int CCupa::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	// Monster State에 따른 Update

	//Attak상태가 아니면서, Regeneration상태도 아니면
	if (Get_State() != MONSTER_STATE::REGEN &&
		Get_State() != MONSTER_STATE::ATTACK)
	{
		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
		if (nullptr == pTarget)
			return S_OK;
		// Player를 타켓으로 Set한다.
		Set_Target(pTarget);
		_vec3 vPlayerPos, vPos, vDir;

		// Player & Monster Positoin
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vDir = vPlayerPos - vPos; // DIrection Vec
		m_vDir = vDir;

		if (D3DXVec3Length(&vDir) < 5.f)
		{
//			if (D3DXVec3Length(&vDir) < 4.f)
//			{
//				Set_State(MONSTER_STATE::REGEN);
//				m_pAnimator->Play_Animation(L"Cupa_Idle_Down", TRUE);
//			}

			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"Cupa_Move_Down", TRUE);
		}
	}
	
	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CCupa::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;
	__super::LateUpdate_Object();
}

void CCupa::Render_Object(void)
{
	if (!Is_Active())
		return;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
}

void CCupa::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"Cupa_Move_Down", TRUE);
		}
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CCupa::Update_Die(_float fTimeDelta)
{
}

void CCupa::Update_Regen(_float fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);

	_vec3 vPlayerPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vDir = vPlayerPos - vPos;

	if (m_fMoveTime > 15.f)
	{
		Set_State(MONSTER_STATE::ATTACK);
		m_pAnimator->Play_Animation(L"Cupa_Attack_Down", TRUE);
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CCupa::Update_Move(_float fTimeDelta)
{
	_vec3 vPlayerPos, vDir, vPos, vDst;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vDir = vPlayerPos - vPos;

	if (m_fMoveTime > 4.f)
	{
//		if (rand() % 10 < 8)
//		{
//			Set_State(MONSTER_STATE::IDLE);
//			m_pAnimator->Play_Animation(L"Cupa_Idle_Down", TRUE);
//		}

		vDst = { float(rand() % 10) - 5.f, 0.f, float(rand() % 10) - 5.f };
		
		
		if (vDst != m_vDst)
			m_vDst = vDst;
		
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;

//	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed() / 2.f);
}

void CCupa::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}

void CCupa::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	vDir = m_vDir;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 2 * Get_Speed()); // Default Speed = 5.f

	if (m_fMoveTime > 5.f)
	{
//		if (rand() % 10 > 8)
//		{
//			Set_State(MONSTER_STATE::IDLE);
//			m_pAnimator->Play_Animation(L"Cupa_Idle_Down", TRUE);
//		}
//		else
//		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"Cupa_Move_Down", TRUE);
			m_fMoveTime = 0.f;
//		}
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

HRESULT CCupa::Add_Component(void)
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

	return S_OK;
}

CCupa* CCupa::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCupa* pInstance = new CCupa(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Cupa Create Failed");
		return nullptr;
	}

	return pInstance;
}
