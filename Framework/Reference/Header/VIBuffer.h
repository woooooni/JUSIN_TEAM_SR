#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	const LPDIRECT3DVERTEXBUFFER9& GetBuffer() { return m_pVB; }
	const LPDIRECT3DINDEXBUFFER9& GetIndex() { return m_pIB; }
	_ulong	GetVtxCount() { return m_dwVtxCnt; }
	_ulong	GetTriangleCount() { return m_dwTriCnt; }
	_ulong	GetFVF() { return m_dwVtxCnt; }

	const _float Get_Width() { return m_fCX; }
	const _float Get_Height() { return m_fCY; }
	const _float Get_Depth() { return m_fCZ; }

	void Set_Width(_float fCX) { m_fCX = fCX; }
	void Set_Height(_float fCY) { m_fCY = fCY; }
	void Set_Depth(_float fCZ) { m_fCZ = fCZ; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong m_dwVtxCnt;
	_ulong m_dwVtxSize;
	_ulong m_dwTriCnt;
	_ulong m_dwFVF;

	_ulong					m_dwIdxSize;
	D3DFORMAT				m_IdxFmt;

	_float m_fCX;
	_float m_fCY;
	_float m_fCZ;


public:
	virtual	void	Free(void);
};

END