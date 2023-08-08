#include "..\..\Header\LightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
	for (_uint i = 0; i < (_uint)LIGHT_TYPE::LIGHT_END; ++i)
	{
		m_arrLight[i] = nullptr;
	}

	ZeroMemory(&m_arrLightInfo, sizeof(D3DLIGHT9) * (_uint)LIGHT_TYPE::LIGHT_END);
	ZeroMemory(&m_arrLightOn, sizeof(bool) * (_uint)LIGHT_TYPE::LIGHT_END);
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
	tDirectionLight.Direction = { 0.0f, -1.0f, 1.0f};
	tDirectionLight.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	tDirectionLight.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	tDirectionLight.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	Ready_Light(pGraphicDev, &tDirectionLight, (_uint)LIGHT_TYPE::LIGHT_DIRECTION);

	//플라워 라이트
	D3DLIGHT9 tFlowerLight;
	ZeroMemory(&tFlowerLight, sizeof(D3DLIGHT9));

	Ready_Light(m_pGraphicDev, &tFlowerLight, (_uint)LIGHT_TYPE::LIGHT_FLOWER1);
	Ready_Light(m_pGraphicDev, &tFlowerLight, (_uint)LIGHT_TYPE::LIGHT_FLOWER2);

	D3DLIGHT9 tSungolemLight;
	ZeroMemory(&tSungolemLight, sizeof(D3DLIGHT9));
	tSungolemLight.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	tSungolemLight.Diffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	tSungolemLight.Ambient = { 0.1f, 0.1f, 0.1f, 1.f };
	tSungolemLight.Specular = { 0.f, 0.f, 0.f, 0.f };
	tSungolemLight.Range = 20.f;
	tSungolemLight.Attenuation0 = 0.001f;	// 상수 감쇠 계수
	tSungolemLight.Attenuation1 = 0.01f;	// 선형 감쇠 계수
	tSungolemLight.Attenuation2 = 0.03f;	// 이차 감쇠 계수
	tSungolemLight.Position = { 9.f, 1.f, 5.f };

	Ready_Light(m_pGraphicDev, &tSungolemLight, (_uint)LIGHT_TYPE::LIGHT_SUNGOLEM);
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
		if (m_arrLight[i] != nullptr)
		{
			m_arrLightInfo[i] = m_arrLight[i]->Get_LightInfo();
			m_arrLightOn[i] = m_arrLight[i]->Is_LightOn();
		}
	}
}

void CLightMgr::Set_LightToEffect(LPD3DXEFFECT _pEffect)
{
	_pEffect->SetValue("g_arrLight", m_arrLightInfo, sizeof(D3DLIGHT9) * (_uint)LIGHT_TYPE::LIGHT_END);
	_pEffect->SetBoolArray("g_arrLightOn", m_arrLightOn, sizeof(bool) * (_uint)LIGHT_TYPE::LIGHT_END);
}

void CLightMgr::Reset_Light()
{
	m_arrLight[(_uint)LIGHT_TYPE::LIGHT_FLOWER1]->Set_LightOff();
	m_arrLight[(_uint)LIGHT_TYPE::LIGHT_FLOWER2]->Set_LightOff();
}

void CLightMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < (_uint)LIGHT_TYPE::LIGHT_END; ++i)
		Safe_Release(m_arrLight[i]);
}
