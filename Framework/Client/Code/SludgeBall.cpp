#include "SludgeBall.h"
#include "Export_Function.h"

CSludgeBall::CSludgeBall(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, OBJ_ID::MONSTER_SKILL)
{
}
CSludgeBall::CSludgeBall(const CSludgeBall& rhs)
	: Engine::CGameObject(rhs)
{

}

CSludgeBall::~CSludgeBall()
{
}

HRESULT CSludgeBall::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SludgeBall", L"Proto_Texture_SludgeBall", 0.1f);
	m_pAnimator->Play_Animation(L"SludgeBall", true);
	m_vDst = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.0f, 1.0f, 1.0f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.0f, 1.0f, 1.0f });
	m_fMinHeight = 0.26f;
	Set_Active(true);
	return S_OK;
}

_int CSludgeBall::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_BULLET);
	m_pAnimator->Play_Animation(L"SludgeBall", true);
	_vec3 vDir, vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst - vPos;
	vDir.y = 0.f;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (Is_Active())
			Set_Active(false);
	}
	else if (vPos.y <= 0.26f)
	{
		if ( m_pAnimator->GetCurrAnimation()->Get_Idx() < 6)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(6);
		
		}
		return iExit;
	}
	else if (m_pAnimator->GetCurrAnimation()->Get_Idx() >6)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(1);
	}
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 7.5f);


	return iExit;
}

void CSludgeBall::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CSludgeBall::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}

HRESULT CSludgeBall::Add_Component(void)
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


CSludgeBall* CSludgeBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSludgeBall* pInstance = new CSludgeBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SludgeBall Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSludgeBall::Free()
{
	__super::Free();
}
void CSludgeBall::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER && m_pShooter->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() < 6)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(6);
		}
	}
}