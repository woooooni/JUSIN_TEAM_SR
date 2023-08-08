#include "..\..\Header\CubeTex.h"



CCubeTex::CCubeTex()
{
}

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CCubeTex::CCubeTex(const CCubeTex & rhs)
	: CVIBuffer(rhs)
{
}


CCubeTex::~CCubeTex()
{
}

HRESULT CCubeTex::Ready_Buffer(void)
{
	m_dwFVF = FVF_CUBE;
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXCUBE*		pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);
	// 전면
	pVertices[0].vPosition = { -1.f, 1.f, -1.f };
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = { 1.f, 1.f, -1.f };
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = { 1.f, -1.f, -1.f };
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = { -1.f, -1.f, -1.f };
	pVertices[3].vTexUV = pVertices[3].vPosition;

	// 후면
	pVertices[4].vPosition = { -1.f, 1.f, 1.f };
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = { 1.f, 1.f, 1.f };
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = { 1.f, -1.f, 1.f };
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = { -1.f, -1.f, 1.f };
	pVertices[7].vTexUV = pVertices[7].vPosition;
	
	m_pVB->Unlock();


	INDEX32*		pIndices = nullptr;
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

void CCubeTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CCubeTex::SetAlpha(_int _iAlpha)
{

}

CCubeTex * CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev , _float fCX, _float fCY, _float fCZ)
{
	CCubeTex *	pInstance = new CCubeTex(pGraphicDev);

	pInstance->Set_Width(fCX);
	pInstance->Set_Height(fCY);
	pInstance->Set_Depth(fCZ);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		
		Safe_Release(pInstance);
		MSG_BOX("CubeTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CCubeTex::Clone(void)
{
	return new CCubeTex(*this);
}

void CCubeTex::Free(void)
{
	CVIBuffer::Free();
}
