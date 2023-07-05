#include "../Include/stdafx.h"
#include "GrabbableObj.h"
#include "Export_Function.h"


CGrabbableObj::CGrabbableObj(LPDIRECT3DDEVICE9 pGraphicDev) : CFieldObject(pGraphicDev)
{
}

CGrabbableObj::CGrabbableObj(const CGrabbableObj& rhs) : CFieldObject(rhs)
{
}

CGrabbableObj::~CGrabbableObj()
{
}

void CGrabbableObj::Grap()
{


}

void CGrabbableObj::UnGrap()
{



}

HRESULT CGrabbableObj::Ready_Object(void)
{	
	FAILED_CHECK(Ready_Component());
	return S_OK;
}

_int CGrabbableObj::Update_Object(const _float& fTimeDelta)
{
	_int ret = __super::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);

	return ret;
}

void CGrabbableObj::LateUpdate_Object(void)
{
}

void CGrabbableObj::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CGrabbableObj::Ready_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GrabStone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;

}

CGrabbableObj* CGrabbableObj::Create(LPDIRECT3DDEVICE9 pDev, const _vec3 p_Pos)
{
	CGrabbableObj* ret = new CGrabbableObj(pDev);

	if (FAILED((ret->Ready_Object())))
	{
		Safe_Release(ret);
		MSG_BOX("GrabObjCreateFailed");
		return nullptr;
	}
	ret->Get_TransformCom()->Set_Pos(&p_Pos);

	return ret;
}
