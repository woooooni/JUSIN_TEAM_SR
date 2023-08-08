#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CRcCubeCol : public CVIBuffer
{
public:
	explicit CRcCubeCol();
	explicit CRcCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCubeCol(const CRcCubeCol& rhs);
	virtual ~CRcCubeCol();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CRcCubeCol* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fCX = 1.f, _float fCY = 1.f, _float fCZ = 1.f);
	virtual CComponent* Clone(void);

private:
	virtual	void	Free(void);
};
END

