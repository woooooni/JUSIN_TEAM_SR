#include "..\..\Header\RcCubeCol.h"



CRcCubeCol::CRcCubeCol()
{
}

CRcCubeCol::CRcCubeCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcCubeCol::CRcCubeCol(const CRcCubeCol& rhs)
	: CVIBuffer(rhs)
{
}


CRcCubeCol::~CRcCubeCol()
{
}

HRESULT CRcCubeCol::Ready_Buffer(void)
{
	m_dwFVF = FVF_CUBECOL;

	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_dwIdxSize = sizeof(INDEX32);
	m_dwTriCnt = 16;

	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE_COL* pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);
	pVertices[0].vPosition = _vec3(-m_fCX * 0.5f, m_fCY * 0.5f, -m_fCZ * 0.5f);
	pVertices[1].vPosition = _vec3(m_fCX * 0.5f, m_fCY * 0.5f, -m_fCZ * 0.5f);
	pVertices[2].vPosition = _vec3(m_fCX * 0.5f, -m_fCY * 0.5f, -m_fCZ * 0.5f);
	pVertices[3].vPosition = _vec3(-m_fCX * 0.5f, -m_fCY * 0.5f, -m_fCZ * 0.5f);

	pVertices[4].vPosition = _vec3(-m_fCX * 0.5f, m_fCY * 0.5f, m_fCZ * 0.5f);
	pVertices[5].vPosition = _vec3(m_fCX * 0.5f, m_fCY * 0.5f, m_fCZ * 0.5f);
	pVertices[6].vPosition = _vec3(m_fCX * 0.5f, -m_fCY * 0.5f, m_fCZ * 0.5f);
	pVertices[7].vPosition = _vec3(-m_fCX * 0.5f, -m_fCY * 0.5f, m_fCZ * 0.5f);

	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[4].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[5].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[6].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertices[7].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

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

void CRcCubeCol::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CRcCubeCol* CRcCubeCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fCX, _float fCY, _float fCZ)
{
	CRcCubeCol* pInstance = new CRcCubeCol(pGraphicDev);

	pInstance->Set_Width(fCX);
	pInstance->Set_Height(fCY);
	pInstance->Set_Depth(fCZ);

	if (FAILED(pInstance->Ready_Buffer()))
	{

		(pInstance);
		MSG_BOX("CubeCol Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CRcCubeCol::Clone(void)
{
	return new CRcCubeCol(*this);
}

void CRcCubeCol::Free(void)
{
	CVIBuffer::Free();
}
