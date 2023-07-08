#include "Player_Skill_Aim.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"

CPlayer_Skill_Aim::CPlayer_Skill_Aim(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER)
{
}

CPlayer_Skill_Aim::CPlayer_Skill_Aim(const CPlayer_Skill_Aim& rhs)
	: Engine::CGameObject(rhs)
{
}

CPlayer_Skill_Aim::~CPlayer_Skill_Aim()
{
}

HRESULT CPlayer_Skill_Aim::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);


	_float m_fAngle;

	m_fAngle = D3DXToRadian(90.0f);
	
	m_pTransformCom->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fAngle);

	m_pAnimator->Add_Animation(L"Aim", L"Proto_Texture_Player_Aim", 0.1f);

	m_pAnimator->Play_Animation(L"Aim", false);

	Set_Active(false);

	return S_OK;
}

_int CPlayer_Skill_Aim::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Skill_Aim::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	__super::LateUpdate_Object();
}

void CPlayer_Skill_Aim::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


HRESULT CPlayer_Skill_Aim::Ready_Component(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

void CPlayer_Skill_Aim::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CPlayer_Skill_Aim::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CPlayer_Skill_Aim::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

CPlayer_Skill_Aim* CPlayer_Skill_Aim::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Skill_Aim* pInstance = new CPlayer_Skill_Aim(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Skill_Range Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Skill_Aim::Free()
{
	__super::Free();
}
