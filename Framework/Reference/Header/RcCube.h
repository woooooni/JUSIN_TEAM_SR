#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CRcCube : public CVIBuffer
{
public:
	explicit CRcCube();
	explicit CRcCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCube(const CRcCube& rhs);
	virtual ~CRcCube();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	void					SetAlpha(_int _iAlpha); // 0 ~ 255

public:
	static CRcCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};
END

