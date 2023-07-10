#include "../Header/PickingMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CPickingMgr)

CPickingMgr::CPickingMgr()
	: m_hWnd(nullptr)
	, m_pGraphicDev(nullptr)
{
}

HRESULT CPickingMgr::Ready_PickingMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd)
{
	m_pGraphicDev = _pDevice;
	_pDevice->AddRef();

	m_hWnd = _hWnd;

	return S_OK;
}

void CPickingMgr::Update_PickingMgr()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_vec3 vMousePos;
	vMousePos.x = _float(pt.x / (WINCX * .5f) - 1);
	vMousePos.y = _float(pt.y / (WINCY * -.5f) + 1);
	vMousePos.z = 0.f;

	_matrix matProjInv; // 프로젝션 역행렬.
	
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProjInv);
	D3DXMatrixInverse(&matProjInv, nullptr, &matProjInv);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProjInv);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePos - m_vRayPos;

	_matrix matViewInv; // 뷰 역행렬.
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matViewInv);
	D3DXMatrixInverse(&matViewInv, nullptr, &matViewInv);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matViewInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matViewInv);
}

void CPickingMgr::Compute_LocalLayInfo(_vec3 * pDir, _vec3 * pRayPos, CTransform * pTransform)
{
	_matrix matWorldInv = pTransform->Get_WorldInverseMatrix();

	D3DXVec3TransformCoord(pRayPos, &m_vRayPos, &matWorldInv);
	D3DXVec3TransformNormal(pDir, &m_vRayDir, &matWorldInv);
}

BOOL CPickingMgr::IsPicking(CGameObject* _pObj, _vec3* _vHit)
{
	CTransform* pTransform = dynamic_cast<CTransform*>(_pObj->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, COMPONENTID::ID_STATIC));
	CVIBuffer*  pBuffer = dynamic_cast<CVIBuffer*>(_pObj->Get_Component(COMPONENT_TYPE::COM_BUFFER, COMPONENTID::ID_STATIC));

	if (nullptr == pBuffer)
		return FALSE;

	if (nullptr == pTransform)
		return FALSE;

	_matrix matWolrd = *(pTransform->Get_WorldMatrix());
	LPDIRECT3DVERTEXBUFFER9 objVB = pBuffer->GetBuffer();
	LPDIRECT3DINDEXBUFFER9	objIB = pBuffer->GetIndex();

	VTXTEX*		pVB;
	INDEX32*	pIB;
	objVB->Lock(0, 0, (void**)&pVB, 0);
	objIB->Lock(0, 0, (void**)&pIB, 0);

	_vec3 vRayDir, vRayPos;
	Compute_LocalLayInfo(&vRayDir, &vRayPos, pTransform);

	D3DXVec3Normalize(&vRayDir, &vRayDir);

	BOOL bRayHit = false;
	_float fU, fV, fDist;
	_float fMinDistance = 999999.f;

	for (_uint i = 0; i < pBuffer->GetTriangleCount(); ++i)
	{
		if (TRUE == D3DXIntersectTri(
			&pVB[pIB[i]._0].vPosition,
			&pVB[pIB[i]._1].vPosition,
			&pVB[pIB[i]._2].vPosition, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			bRayHit = TRUE;
			if(fDist < fMinDistance)
			{
				_vec3 vPickPos = vRayPos + vRayDir * fDist;
				D3DXVec3TransformCoord(_vHit, &vPickPos, &matWolrd);
			}
		}
	}

	objVB->Unlock();
	objIB->Unlock();

	return bRayHit;
}

void CPickingMgr::Free()
{
	Safe_Release(m_pGraphicDev);
}


