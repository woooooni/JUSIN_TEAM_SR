#pragma once

#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	HRESULT Ready_LightMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT	Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
		const D3DLIGHT9* pLightInfo,
		const _uint& iIndex);

	void	LateUpdate_LightMgr();


	void Light_On(LIGHT_TYPE _eLightType) { m_arrLight[_uint(_eLightType)]->Set_LightOn(); }
	void Light_Off(LIGHT_TYPE _eLightType) { m_arrLight[_uint(_eLightType)]->Set_LightOff(); }

	CLight* Get_Light(LIGHT_TYPE _eLightType) { return m_arrLight[(_uint)_eLightType]; }

	void Set_LightToEffect(LPD3DXEFFECT _pEffect);
private:
	CLight*	m_arrLight[(_uint)LIGHT_TYPE::LIGHT_END];
	D3DLIGHT9 m_arrLightInfo[(_uint)LIGHT_TYPE::LIGHT_END];
	BOOL m_arrLightOn[(_uint)LIGHT_TYPE::LIGHT_END];

	LPDIRECT3DDEVICE9 m_pGraphicDev;


private:
	virtual void Free(void);
};

END