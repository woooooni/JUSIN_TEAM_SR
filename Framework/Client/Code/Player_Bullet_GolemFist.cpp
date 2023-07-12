#include "Player_Bullet_GolemFist.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"

CPlayer_Bullet_GolemFist::CPlayer_Bullet_GolemFist(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER, OBJ_ID::PLAYER_BULLET_GOLEMFIST), m_fStopTime(0.1f), m_fAccTime(0.0f)
{
}

CPlayer_Bullet_GolemFist::CPlayer_Bullet_GolemFist(const CPlayer_Bullet_GolemFist& rhs)
    : Engine::CGameObject(rhs)
{
}

CPlayer_Bullet_GolemFist::~CPlayer_Bullet_GolemFist()
{
}

HRESULT CPlayer_Bullet_GolemFist::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(0.8f, 0.8f, 0.8f));

	m_pAnimator->Add_Animation(L"Fist", L"Proto_Texture_SunGolem_Idle_Fist", 0.1f);

	m_pAnimator->Play_Animation(L"Fist", false);

	Set_Active(false);

	return S_OK;
}

_int CPlayer_Bullet_GolemFist::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pRigidBodyCom->IsGround())
	{
		m_fAccTime += fTimeDelta;

		if (m_fAccTime > m_fStopTime)
		{
			m_fAccTime = 0.0f;
			Set_Active(false);
		}
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Bullet_GolemFist::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	
		

	__super::LateUpdate_Object();
}

void CPlayer_Bullet_GolemFist::Render_Object(void)
{
	if (!Is_Active())
		return;

	

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CPlayer_Bullet_GolemFist::Ready_Component(void)
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

void CPlayer_Bullet_GolemFist::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_GolemFist::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_GolemFist::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CPlayer_Bullet_GolemFist* CPlayer_Bullet_GolemFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Bullet_GolemFist* pInstance = new CPlayer_Bullet_GolemFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Bullet_GolemFist_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Bullet_GolemFist::Free()
{
	__super::Free();
}
