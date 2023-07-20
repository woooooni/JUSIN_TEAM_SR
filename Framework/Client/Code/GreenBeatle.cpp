#include "GreenBeatle.h"
#include "Export_Function.h"

CGreenBeatle::CGreenBeatle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::GREEN_BEATLE), m_fMoveTime(0.f)
{
}

CGreenBeatle::CGreenBeatle(const CGreenBeatle& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CGreenBeatle::~CGreenBeatle()
{
}


HRESULT CGreenBeatle::Ready_Object(void)
{
	Set_State(MONSTER_STATE::IDLE);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"GreenBeatle_Idle_Down", L"Proto_Texture_GreenBeatle_Idle_Down", 0.1f);
	m_pAnimator->Add_Animation(L"GreenBeatle_Move_Down", L"Proto_Texture_GreenBeatle_Move_Down", 0.1f);

	m_pTransformCom->Set_Pos(&_vec3(10.0f, 1.0f, 10.0f));
	Set_Speed(5.f);

	m_pAnimator->Play_Animation(L"GreenBeatle_Idle_Down", true);
	m_fMinHeight = 0.5f;
	m_tStat = { 3,3,0 };

	m_pUIBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	if (m_pUIBack != nullptr)
		m_pUIBack->Set_Owner(this);

	m_pUIGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	if (m_pUIGauge != nullptr)
		m_pUIGauge->Set_Owner(this);

	m_pUIFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	if (m_pUIFrame != nullptr)
		m_pUIFrame->Set_Owner(this);

	return S_OK;
}


_int CGreenBeatle::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vPos.y += 0.5f;
	vPos.z -= 0.01f;

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Update_Object(fTimeDelta);
		m_pUIBack->Get_TransformCom()->Set_Pos(&vPos);

		vPos.z -= 0.005f;
		m_pUIGauge->Update_Object(fTimeDelta);

		if (m_tStat.iHp == m_tStat.iMaxHp)
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vPos);
		else if (m_tStat.iHp > 0 && m_tStat.iHp < m_tStat.iMaxHp)
		{
			_vec3 vMovePos = vPos;

			_float fMaxHP = _float(m_tStat.iMaxHp);
			_float fCurHP = _float(m_tStat.iHp);
			_float fHP = fCurHP / fMaxHP;

			_float fOriginWidth = _float(m_pUIGauge->Get_TextureCom()->Get_TextureDesc(0).Width);
			_float fWidth = fOriginWidth - fOriginWidth * fHP;

			_float fIndex = fWidth * 0.004f * 0.5f;

			vMovePos = _vec3((vMovePos.x - fIndex), vMovePos.y, vMovePos.z);
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vMovePos);
		}

		vPos.z -= 0.005f;
		m_pUIFrame->Update_Object(fTimeDelta);
		m_pUIFrame->Get_TransformCom()->Set_Pos(&vPos);
	}

	return iExit;
}

void CGreenBeatle::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;
	__super::LateUpdate_Object();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->LateUpdate_Object();
		m_pUIGauge->LateUpdate_Object();
		m_pUIFrame->LateUpdate_Object();
	}
}

void CGreenBeatle::Render_Object(void)
{
	if (!Is_Active())
		return ;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}
}


void CGreenBeatle::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"GreenBeatle_Move_Down", true);
		}
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CGreenBeatle::Update_Regen(_float fTimeDelta)
{
}


void CGreenBeatle::Update_Attack(_float fTimeDelta)
{
}

void CGreenBeatle::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;

	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"GreenBeatle_Idle_Down", true);
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

void CGreenBeatle::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
		Set_Active(false);
}


HRESULT CGreenBeatle::Add_Component(void)
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

CGreenBeatle* CGreenBeatle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGreenBeatle* pInstance = new CGreenBeatle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("RedBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CGreenBeatle::Trace(_float fTimeDelta)
{
}

void CGreenBeatle::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;
	__super::Collision_Enter(pCollider, _eCollisionGroup, _iColliderID);


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