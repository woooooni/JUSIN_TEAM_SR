#include "TrashSlime.h"
#include "Export_Function.h"

CTrashSlime::CTrashSlime(LPDIRECT3DDEVICE9 pGraphicDev) :CMonster(pGraphicDev)
{
}

CTrashSlime::CTrashSlime(const CTrashSlime& rhs) : CMonster(rhs)
{
}

CTrashSlime::~CTrashSlime()
{
}

void CTrashSlime::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::MOVE);
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
		}
			m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CTrashSlime::Update_Die(_float fTimeDelta)
{
	//if (m_pAnimator)
}

void CTrashSlime::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
		}
	}

}

void CTrashSlime::Update_Move(_float fTimeDelta)
{
	_vec3 vDir, vPos, vDst;
	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
			m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
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

void CTrashSlime::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}

HRESULT CTrashSlime::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Down",		L"Proto_Texture_TrashSlime_Idle_Down", 0.2f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Up",			L"Proto_Texture_TrashSlime_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Left",		L"Proto_Texture_TrashSlime_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_Right",		L"Proto_Texture_TrashSlime_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_RightDown",	L"Proto_Texture_TrashSlime_Idle_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_RightUp",	L"Proto_Texture_TrashSlime_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_LeftDown",	L"Proto_Texture_TrashSlime_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Idle_LeftUp",		L"Proto_Texture_TrashSlime_Idle_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Down",		L"Proto_Texture_TrashSlime_Move_Down", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Up",			L"Proto_Texture_TrashSlime_Move_Up", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Left",		L"Proto_Texture_TrashSlime_Move_Left", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_Right",		L"Proto_Texture_TrashSlime_Move_Right", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_RightDown",	L"Proto_Texture_TrashSlime_Move_RightDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_RightUp",	L"Proto_Texture_TrashSlime_Move_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_LeftDown",	L"Proto_Texture_TrashSlime_Move_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"TrashSlime_Move_LeftUp",		L"Proto_Texture_TrashSlime_Move_LeftUp", 0.1f);
	
	m_pAnimator->Add_Animation(L"TrashSlime_Regen_Down", L"Proto_Texture_TrashSlime_Regen_Down", 0.1f);
	m_pTransformCom->Set_Pos(&_vec3(5.0f, 1.0f, 5.0f));
	Set_Speed(2.f);
	Set_State(MONSTER_STATE::REGEN);
	m_pAnimator->Play_Animation(L"TrashSlime_Regen_Down", false);

	return S_OK;
}

_int CTrashSlime::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_MONSTER);
	if (MONSTER_STATE::ATTACK != Get_State())
	{
		CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
		NULL_CHECK_RETURN(pTarget, -1 );
		Set_Target(pTarget);
		_vec3 vTargetPos, vPos, vDir;
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vTargetPos - vPos;
		if (D3DXVec3Length(&vDir) < 5.f)
		{
			Set_State(MONSTER_STATE::ATTACK);
			m_pAnimator->Play_Animation(L"TrashSlime_Move_Down", true);
		}
	}
	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CTrashSlime::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTrashSlime::LateUpdate_Object(void)
{

		__super::LateUpdate_Object();
}

HRESULT CTrashSlime::Add_Component(void)
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
	return S_OK;
}

CTrashSlime* CTrashSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrashSlime* pInstance = new CTrashSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BlueBeatle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTrashSlime::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.f;
	if (D3DXVec3Length(&vDir) > 5.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_pAnimator->Play_Animation(L"TrashSlime_Idle_Down", true);
		return;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());

}
void CTrashSlime::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::ATTACK || Get_State() == MONSTER_STATE::DIE)
		return;
	//	if(pCollider->GetOwner()->)
	//if (dynamic_cast<CPushStone*>(pCollider->GetOwner()))
	//{
	//	if (dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Flying() == true)
	//	{
	//		m_tStat.iHp -= 1.f;
	//		//MSG_BOX("보스 피격");
	//	}
	//}
}