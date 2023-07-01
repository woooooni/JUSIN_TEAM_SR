#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Function.h"
#include "TriCol.h"
#include "Transform.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT)
	, m_vDir(0.f, 0.f, 0.f)
	, m_fSpeed(5.f)
{

}

CBullet::CBullet(const CBullet & rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDir(0.f, 0.f, 0.f)
{
	m_vDir = rhs.m_vDir;
	//m_pBufferCom	= rhs.m_pBufferCom->Clone();
	//m_pTransformCom = rhs.m_pTransformCom->Clone();
	//m_pColliderCom = nullptr;
}


CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vDir = _vec3(0.f, 0.f, 0.f);
	return S_OK;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 10.f);
	return iExit;
}

void CBullet::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBullet::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet* pInstance = new CBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Bullet Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CBullet::Free()
{
	__super::Free();
}
