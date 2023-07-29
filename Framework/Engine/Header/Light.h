#pragma once


#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CLight();

public:
	HRESULT				Ready_Light(const D3DLIGHT9* pLight);


public:
	// 수정 가능.
	D3DLIGHT9& Get_LightInfo() { return m_tLightInfo; }
	void Set_LightInfo(const D3DLIGHT9& tLightInfo) { m_tLightInfo = tLightInfo; }

	_bool Is_LightOn() { return m_bLightOn; }

	void Set_LightOn() { m_bLightOn = true; }
	void Set_LightOff() { m_bLightOn = false; }

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	D3DLIGHT9 m_tLightInfo;
	_bool m_bLightOn;

	
public:
	static CLight*		Create(LPDIRECT3DDEVICE9 _pDevice,
								const D3DLIGHT9* pLightInfo);

private:
	virtual void Free();

};
END
