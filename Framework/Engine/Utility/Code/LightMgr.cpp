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

	return S_OK;
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{

	CLight* pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	if (m_arrLight[iIndex])
		Safe_Release(m_arrLight[iIndex]);
	
	m_arrLight[iIndex] = pLight;

	return S_OK;
}

void CLightMgr::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < (_uint)LIGHT_TYPE::LIGHT_END; ++i)
		Safe_Release(m_arrLight[i]);
}
