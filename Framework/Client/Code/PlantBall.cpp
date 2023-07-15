#include "PlantBall.h"
#include "Export_Function.h"

CPlantBall::CPlantBall(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, OBJ_ID::MONSTER_SKILL)
{
}
CPlantBall::CPlantBall(const CPlantBall& rhs)
	: Engine::CGameObject(rhs)
{

}

CPlantBall::~CPlantBall()
{
}

HRESULT CPlantBall::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"PlantBall", L"Proto_Texture_PlantBall", 0.1f);
	m_pAnimator->Play_Animation(L"PlantBall", true);
	m_vDir = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({0.5f, 0.5f, 0.5f });
	Set_Active(true);
	return S_OK;
}

_int CPlantBall::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_BULLET);
		m_pAnimator->Play_Animation(L"PlantBall", true);

	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 5.f);
	if (m_fMoveTime < 0.f)
	{
		if (Is_Active())
			Set_Active(false);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime -= 10.f * fTimeDelta;
	return iExit;
}

void CPlantBall::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y < 1.f)
	{
		if (Is_Active())
			Set_Active(false);
		m_fMoveTime = 0.f;
	}
	__super::LateUpdate_Object();


}

void CPlantBall::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}

HRESULT CPlantBall::Add_Component(void)
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


CPlantBall* CPlantBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlantBall* pInstance = new CPlantBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("PlantBall Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CPlantBall::Free()
{
	__super::Free();
}
void CPlantBall::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER && m_pShooter->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		if (Is_Active())
			Set_Active(false);
	}
}