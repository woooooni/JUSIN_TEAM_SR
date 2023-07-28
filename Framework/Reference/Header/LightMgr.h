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

	void Light_On(LIGHT_TYPE _eLightType) { m_pGraphicDev->LightEnable(_uint(_eLightType), TRUE); }
	void Light_Off(LIGHT_TYPE _eLightType) { m_pGraphicDev->LightEnable(_uint(_eLightType), FALSE); }

	CLight* Get_Light(LIGHT_TYPE _eLightType) { return m_arrLight[(_uint)_eLightType]; }
	

private:
	CLight*	m_arrLight[(_uint)LIGHT_TYPE::LIGHT_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;


private:
	virtual void Free(void);
};

END