#include "..\..\Header\BoxCollider.h"
#include "Export_Function.h"
#include "KeyMgr.h"


_bool g_bRender = false;
CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CCollider(_pDevice, COMPONENT_TYPE::COM_BOX_COLLIDER, COLLIDER_TYPE::COLLIDER_BOX)
{

}

CBoxCollider::CBoxCollider(const CBoxCollider & rhs)
	: CCollider(rhs)
{
	Ready_BoxCollider();
}


CBoxCollider::~CBoxCollider()
{

}


HRESULT CBoxCollider::Ready_BoxCollider()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8, 0, D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0), D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32) * 16, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	VTXCUBE* pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;
	pVertices[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;
	pVertices[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;
	pVertices[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;
	pVertices[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;
	pVertices[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;
	pVertices[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;


	m_pVB->Unlock();

	INDEX32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* +x */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* -x */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* +y */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* -y */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* +z */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* -z */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;


	m_pIB->Unlock();

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
	if (!g_bRender)
		return;

	_matrix matPrevMatrix;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matPrevMatrix);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	*(_vec3*)(&matWorld.m[0][0]) = _vec3{ 1.f * m_vScale.x, 0.f, 0.f };
	*(_vec3*)(&matWorld.m[1][0]) = _vec3{ 0.f, 1.f * m_vScale.y, 0.f };
	*(_vec3*)(&matWorld.m[2][0]) = _vec3{ 0.f, 0.f, 1.f * m_vScale.z };
	*(_vec3*)(&matWorld.m[3][0]) = m_vCenterPos;

	m_pGraphicDev->SetTexture(0, nullptr);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 16);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPrevMatrix);
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
	CComponent::Free();
}

CComponent* CBoxCollider::Clone(void)
{
	return new CBoxCollider(*this);
}

void CBoxCollider::InputCollider()
{
	if (KEY_TAP(KEY::F2))
		g_bRender = !g_bRender;
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
