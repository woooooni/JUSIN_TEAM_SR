#include "..\..\Header\BoxCollider.h"
#include "Export_Function.h"

CBoxCollider::CBoxCollider()
{
}

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CCollider(_pDevice, COMPONENT_TYPE::COM_BOX_COLLIDER, COLLIDER_TYPE::COLLIDER_BOX)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider & rhs)
	: CCollider(rhs)
{
	m_pBuffer = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_RcCube"));
}


CBoxCollider::~CBoxCollider()
{
}


HRESULT CBoxCollider::Ready_BoxCollider()
{
	m_pBuffer = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_RcCube"));
	NULL_CHECK_RETURN_MSG(m_pBuffer, E_FAIL, L"Clone Proto RcCube Failed");

	return S_OK;
}

_int CBoxCollider::Update_Component(const _float & fTimeDelta)
{
	CTransform* pOwnerTransform = (CTransform*)(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, COMPONENTID::ID_STATIC));


	pOwnerTransform->Get_Info(INFO_POS, &m_vCenterPos);
	const D3DXMATRIX& matWorld = *pOwnerTransform->Get_WorldMatrix();


	for (UINT i = 0; i < 3; ++i)
	{
		m_vAxisDir[i] *= m_fAxisLen[i];
		memcpy(&m_vAxisDir[i], &matWorld.m[i][0], sizeof(_vec3));
	}

	return S_OK;
}

void CBoxCollider::LateUpdate_Component()
{
	CCollider::LateUpdate_Component();
}

void CBoxCollider::Render_Component()
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < 3; ++i)
	{
		m_vAxisDir[i] *= m_fAxisLen[i];
		memcpy(&matWorld.m[i][0], &(m_vAxisDir[i]), sizeof(_vec3));
	}
		
	memcpy(&matWorld.m[3][0], &m_vCenterPos, sizeof(_vec3));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

CBoxCollider * CBoxCollider::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBoxCollider*		pInstance = new CBoxCollider(_pDevice);

	if (FAILED(pInstance->Ready_BoxCollider()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BoxCollider Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoxCollider::Free()
{
	Safe_Release(m_pBuffer);
	CComponent::Free();
}

CComponent * CBoxCollider::Clone(void)
{
	return new CBoxCollider(*this);
}

void CBoxCollider::OnCollisionEnter(CCollider * _pOther)
{
	m_pOwner->Collision_Enter(_pOther->GetOwner());
}

void CBoxCollider::OnCollisionStay(CCollider * _pOther)
{
	m_pOwner->Collision_Stay(_pOther->GetOwner());
}

void CBoxCollider::OnCollisionExit(CCollider * _pOther)
{
	m_pOwner->Collision_Exit(_pOther->GetOwner());
}
