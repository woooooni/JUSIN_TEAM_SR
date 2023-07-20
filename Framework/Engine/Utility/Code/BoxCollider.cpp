#include "..\..\Header\BoxCollider.h"
#include "Export_Function.h"
#include "KeyMgr.h"

CBoxCollider::CBoxCollider()
	: m_pMesh(nullptr)
{
}

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CCollider(_pDevice, COMPONENT_TYPE::COM_BOX_COLLIDER, COLLIDER_TYPE::COLLIDER_BOX)
	, m_pMesh(nullptr)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider & rhs)
	: CCollider(rhs)
	, m_pMesh(rhs.m_pMesh)
{

}


CBoxCollider::~CBoxCollider()
{
}


HRESULT CBoxCollider::Ready_BoxCollider()
{
	FAILED_CHECK_RETURN(D3DXCreateBox(m_pGraphicDev, m_vScale.x, m_vScale.y, m_vScale.z, &m_pMesh, nullptr), E_FAIL);
	int i = 0;
	return S_OK;
}

_int CBoxCollider::Update_Component(const _float & fTimeDelta)
{
	CTransform* pOwnerTransform = m_pOwner->Get_TransformCom();

	if (nullptr == pOwnerTransform)
		return S_OK;

	_vec3 vPos;
	pOwnerTransform->Get_Info(INFO_POS, &vPos);
	m_vCenterPos = vPos + m_vOffset;

	InputCollider();

	return S_OK;
}

void CBoxCollider::LateUpdate_Component()
{
	
}

void CBoxCollider::Render_Component()
{
	/*if (!m_bRender)
		return;

	CTransform* pOwnerTransform = (CTransform*)(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, COMPONENTID::ID_STATIC));

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld = *(m_pOwner->Get_TransformCom()->Get_WorldMatrix());
	
	matWorld._11 = m_vScale.x;
	matWorld._22 = m_vScale.y;
	matWorld._33 = m_vScale.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
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
	if (!m_bClone)
		m_pMesh->Release();

	CComponent::Free();
}

CComponent* CBoxCollider::Clone(void)
{
	return new CBoxCollider(*this);
}

void CBoxCollider::InputCollider()
{
	if (KEY_TAP(KEY::F2))
		m_bRender = !m_bRender;

}

void CBoxCollider::OnCollisionEnter(CCollider * _pOther, COLLISION_GROUP _eGroup)
{
	m_pOwner->Collision_Enter(_pOther, _eGroup, Get_Id());
}

void CBoxCollider::OnCollisionStay(CCollider * _pOther, COLLISION_GROUP _eGroup)
{
	m_pOwner->Collision_Stay(_pOther, _eGroup, Get_Id());
}

void CBoxCollider::OnCollisionExit(CCollider * _pOther, COLLISION_GROUP _eGroup)
{
	m_pOwner->Collision_Exit(_pOther, _eGroup, Get_Id());
}
