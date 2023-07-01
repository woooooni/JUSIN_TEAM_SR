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

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&matView, nullptr, &matView);


	m_pTransformCom->Set_Info(INFO_RIGHT, &(*(_vec3*)&matView.m[0][0] * fScale.x));
	m_pTransformCom->Set_Info(INFO_UP, &(*(_vec3*)&matView.m[1][0] * fScale.y));
	m_pTransformCom->Set_Info(INFO_LOOK, &(*(_vec3*)&matView.m[2][0] * fScale.z));
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
}
