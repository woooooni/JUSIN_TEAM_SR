#include "..\Header\Monster.h"
#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
{

}
CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs)
{

}
CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);


	return iExit;
}

void CMonster::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CMonster::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return S_OK;
}

void CMonster::Trace()
{
}

void CMonster::Free()
{
	__super::Free();
}
