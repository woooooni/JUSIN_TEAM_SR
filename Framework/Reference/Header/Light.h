#pragma once


#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CLight();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	D3DLIGHT9			m_tLightInfo;
	_uint				m_iIndex;

public:
	HRESULT				Ready_Light(const D3DLIGHT9* pLight, const _uint& iIndex);

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 _pDevice,
								const D3DLIGHT9* pLightInfo,
								const _uint& iIndex);

private:
	virtual void Free();

};
END
