#include "..\..\Header\LightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
	for (_uint i = 0; i < (_uint)LIGHT_TYPE::LIGHT_END; ++i)
	{
		m_arrLight[i] = nullptr;
	}
}


CLightMgr::~CLightMgr()
{
	Free();
}

HRESULT CLightMgr::Ready_LightMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	pGraphicDev->AddRef();

	D3DLIGHT9 tDirectionLight;
	ZeroMemory(&tDirectionLight, sizeof(D3DLIGHT9));

	tDirectionLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tDirectionLight.Direction = { 0.f, -1.f, 1.f};
	tDirectionLight.Ambient = { 0.2f, 0.2f, 0.2f, 1.f };
	tDirectionLight.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tDirectionLight.Specular = { 0.2f, 0.2f, 0.2f, 1.f };

	Ready_Light(pGraphicDev, &tDirectionLight, (_uint)LIGHT_TYPE::LIGHT_DIRECTION);

	return S_OK;
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{

	CLight* pLight = CLight::Create(pGraphicDev, pLightInfo);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	if (m_arrLight[iIndex])
		Safe_Release(m_arrLight[iIndex]);
	
	m_arrLight[iIndex] = pLight;
	m_arrLightInfo[iIndex] = m_arrLight[iIndex]->Get_LightInfo();

	return S_OK;
}

void CLightMgr::LateUpdate_LightMgr()
{
	for (_uint i = 0; (_uint)LIGHT_TYPE::LIGHT_END > i; ++i)
	{
		m_arrLightInfo[i] = m_arrLight[i]->Get_LightInfo();
		m_arrLightOn[i] = m_arrLight[i]->Is_LightOn();
	}
}

void CLightMgr::Set_LightToEffect(LPD3DXEFFECT _pEffect)
{
	_pEffect->SetValue("g_arrLight", m_arrLightInfo, sizeof(D3DLIGHT9) * (_uint)LIGHT_TYPE::LIGHT_END);
	_pEffect->SetBoolArray("g_arrLightOn", m_arrLightOn, sizeof(bool) * (_uint)LIGHT_TYPE::LIGHT_END);
}

void CLightMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < (_uint)LIGHT_TYPE::LIGHT_END; ++i)
		Safe_Release(m_arrLight[i]);
}
