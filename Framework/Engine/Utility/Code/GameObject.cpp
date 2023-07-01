#include "..\..\Header\GameObject.h"
#include "Component.h"
#include "Transform.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_TYPE _eType)
	: m_pGraphicDev(pGraphicDev)
	, m_eType(_eType)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_eType(rhs.m_eType)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{

}


HRESULT CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);
	
	return 0;
}

void CGameObject::LateUpdate_Object(void)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_Object(void)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Render_Component();
}

CComponent* CGameObject::Get_Component(COMPONENT_TYPE eType, COMPONENTID eID)
{
	CComponent* pComponent = Find_Component(eType, eID);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

void CGameObject::Set_Billboard()
{
	if (m_pTransformCom == nullptr)
		return;

	_vec3 fScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vLookTemp);

	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCameraPos = _vec3(matView._41, matView._42, matView._43);
	_vec3 vObjPos, vObjUp;
	m_pTransformCom->Get_Info(INFO_UP, &vObjUp);
	m_pTransformCom->Get_Info(INFO_POS, &vObjPos);

	_vec3 vDir = vCameraPos - vObjPos;
	_vec3 vRight, vLook;
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Cross(&vRight, &vObjUp, &vDir);
	D3DXVec3Cross(&vLook, &vRight, &vObjUp);
	m_pTransformCom->Set_Info(INFO_RIGHT, &(vRight));
	m_pTransformCom->Set_Info(INFO_LOOK, &(vLook));

}

CComponent * CGameObject::Find_Component(COMPONENT_TYPE eType, COMPONENTID eID)
{
	auto& iter = m_mapComponent[eID].find(eType);

	if (iter != m_mapComponent[eID].end())
		return iter->second;

	return nullptr;
}

void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pAnimator);
}
