#include "Nexus.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"



CNexus::CNexus(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, OBJ_ID::NEAR_REACT)
{
}

CNexus::CNexus(const CNexus& rhs)
	: CGameObject(rhs)
{
}

CNexus::~CNexus()
{
}

HRESULT CNexus::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.1f));

	m_fMinHeight = 0.2f;

	return S_OK;
}

_int CNexus::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	//Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_NEXUS);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CNexus::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNexus::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture();
	__super::Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CNexus::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CNexus::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CNexus::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CNexus::Ready_Component(void)
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


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CNexus* CNexus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNexus* pInstance = new CNexus(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Nexus_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNexus::Free()
{
	__super::Free();
}
