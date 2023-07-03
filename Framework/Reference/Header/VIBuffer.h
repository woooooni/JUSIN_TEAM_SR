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

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong m_dwVtxCnt;
	_ulong m_dwVtxSize;
	_ulong m_dwTriCnt;
	_ulong m_dwFVF;

	_ulong					m_dwIdxSize;
	D3DFORMAT				m_IdxFmt;


public:
	virtual	void	Free(void);
};

END