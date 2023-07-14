#include  "../Include/stdafx.h"
#include "PushStone.h"
#include "Export_Function.h"

CPushStone::CPushStone(LPDIRECT3DDEVICE9 pDev) : CFieldObject(pDev, OBJ_ID::PUSH_STONE) , m_bIsFlying(false)
{

}

CPushStone::CPushStone(const CPushStone& rhs) : CFieldObject(rhs), m_bIsFlying(rhs.m_bIsFlying)
{
}

CPushStone::~CPushStone()
{
}

HRESULT CPushStone::Ready_Object(void)
{

	FAILED_CHECK(Ready_Component());

	CComponent* pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ COMPONENT_TYPE::COM_RIGIDBODY, pComponent });

	Set_MinHeight(0.5f);

    return S_OK;
}

_int CPushStone::Update_Object(const _float& fTimeDelta)
{
	if (Is_Active())
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
		Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		_vec3 src;
		m_pTransformCom->Get_Info(INFO_POS, &src);
		if (m_bIsFlying && m_pRigidBodyCom->IsGround())
		{
			m_bIsFlying = false;
			m_pRigidBodyCom->SetVelocity({ 0, 0, 0 });
			m_pRigidBodyCom->SetGround(true);
		}

	}


	
    return	__super::Update_Object(fTimeDelta);
	
}

void CPushStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CPushStone::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	

	m_pTextureCom->Render_Texture();

	m_pBufferCom->Render_Buffer();

	m_pColliderCom->Render_Component();
}



void CPushStone::Free()
{
	__super::Free();

}

CPushStone* CPushStone::Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPushStone* pInstance = new CPushStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	pInstance->m_pTransformCom->Set_Pos(&p_Pos);

	return pInstance;

}

void CPushStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER)
		return;

	if(!m_bIsFlying)
		Push_Me(pCollider);
	m_pColliderCom->Update_Component(0.f);
}

HRESULT CPushStone::Ready_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;

}


