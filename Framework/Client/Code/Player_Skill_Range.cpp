#include "Player_Skill_Range.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"

CPlayer_Skill_Range::CPlayer_Skill_Range(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER)
{
}

CPlayer_Skill_Range::CPlayer_Skill_Range(const CPlayer_Skill_Range& rhs)
	: Engine::CGameObject(rhs)
{
}

CPlayer_Skill_Range::~CPlayer_Skill_Range()
{
}

HRESULT CPlayer_Skill_Range::Ready_Object(void)
{
	

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	D3DXMatrixIdentity(&m_matReset);



	m_pAnimator->Add_Animation(L"SkillRange", L"Proto_Texture_Player_SkillRange", 0.1f);
	
	m_pAnimator->Play_Animation(L"SkillRange",false);

	Set_Active(false);

	return S_OK;
}

_int CPlayer_Skill_Range::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Skill_Range::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	__super::LateUpdate_Object();
}

void CPlayer_Skill_Range::Render_Object(void)
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

void CPlayer_Skill_Range::Reset_Range(void)
{
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &m_matReset.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}
}

void CPlayer_Skill_Range::Set_Range(_vec3& _vScale, _vec3& _vPos)
{
	_float m_fRadian = D3DXToRadian(90.0f);
	Reset_Range();
	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fRadian);
	m_pTransformCom->Set_Pos(&_vPos);
}

HRESULT CPlayer_Skill_Range::Ready_Component(void)
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

void CPlayer_Skill_Range::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Skill_Range::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Skill_Range::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CPlayer_Skill_Range* CPlayer_Skill_Range::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Skill_Range* pInstance = new CPlayer_Skill_Range(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Skill_Range Create Failed");
		return nullptr;
	}

	return pInstance;
}


void CPlayer_Skill_Range::Free()
{
	__super::Free();
}
