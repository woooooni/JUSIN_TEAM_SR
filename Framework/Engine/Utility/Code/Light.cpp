#include "..\..\Header\Light.h"
#include "Export_Function.h"

CLight::CLight(LPDIRECT3DDEVICE9 _pDevice)
    : m_pGraphicDev(_pDevice)
    , m_bLightOn(true)
{
    ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
    m_pGraphicDev->AddRef();
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLight)
{
    memcpy(&m_tLightInfo, pLight, sizeof(D3DLIGHT9));

    return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* pLightInfo)
{
    CLight* pInstance = new CLight(_pDevice);

    if (FAILED(pInstance->Ready_Light(pLightInfo)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Light Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CLight::Free()
{
    Safe_Release(m_pGraphicDev);
}
