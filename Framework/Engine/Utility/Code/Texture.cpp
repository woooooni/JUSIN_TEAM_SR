#include "Texture.h"



CTexture::CTexture()
	: m_iIdx()
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice)
	:CComponent(_pDevice, COMPONENT_TYPE::COM_TEXTURE)
	, m_iIdx(0)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_iIdx(rhs.m_iIdx)
{
	m_vecTexture = rhs.m_vecTexture;
}


CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTUREID _eType, const _tchar * _pPath, const _uint & iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, _pPath, i);

		switch (_eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}
		
		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void CTexture::Render_Texture(const _uint & iIndex)
{
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iIdx]);
}

void CTexture::Set_Idx(_uint _iIdx)
{
	m_iIdx = _iIdx;
	if (m_iIdx >= m_vecTexture.size())
		m_iIdx = 0;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 _pDevice, TEXTUREID eType, _tchar * pPath, const _uint & iCnt)
{
	CTexture*			pInstance = new CTexture(_pDevice);

	if (FAILED(pInstance->Ready_Texture(eType, pPath, iCnt)))
	{
		MSG_BOX("Texture Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();

	CComponent::Free();
}
