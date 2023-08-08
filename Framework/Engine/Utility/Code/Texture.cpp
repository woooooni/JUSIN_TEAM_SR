#include "Texture.h"



CTexture::CTexture()
	: m_iIdx(0)
	, m_fAccTime(0.f)
	, m_fFrameTime(0.f)
	, m_bFinished(false)
	, m_bLoop(true)
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice)
	:CComponent(_pDevice, COMPONENT_TYPE::COM_TEXTURE)
	, m_iIdx(0)
	, m_fAccTime(0.f)
	, m_fFrameTime(0.f)
	, m_bFinished(false)
	, m_bLoop(true)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_iIdx(rhs.m_iIdx)
	, m_fAccTime(0.f)
	, m_fFrameTime(rhs.m_fFrameTime)
	, m_bFinished(false)
	, m_bLoop(rhs.m_bLoop)
{
	m_vecTexture = rhs.m_vecTexture;
	m_vecTextureInfo = rhs.m_vecTextureInfo;
}


CTexture::~CTexture()
{
}

_int CTexture::Update_Component(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if (m_fAccTime >= m_fFrameTime)
	{
		m_fAccTime = 0.f;
		m_iIdx++;
		
		if (m_iIdx >= m_vecTexture.size())
		{           
			m_bLoop ? m_iIdx = 0 : m_iIdx = m_vecTexture.size() - 1;
			m_bFinished = true;
		}
	}
	return S_OK;
}

HRESULT CTexture::Ready_Texture(TEXTUREID _eType, const _tchar * _pPath, const _uint & iCnt)
{
	m_vecTexture.reserve(iCnt);
	m_vecTextureInfo.reserve(iCnt);

	D3DXIMAGE_INFO tInfo;
	ZeroMemory(&tInfo, sizeof(D3DXIMAGE_INFO));
	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, _pPath, i);

		switch (_eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFileEx(
				m_pGraphicDev, 
				szFileName,
				0, 0,
				0,
				0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				&tInfo,
				NULL,
				(LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFileEx(
				m_pGraphicDev
				,szFileName
				,0 ,0
				,0
				, D3DFMT_UNKNOWN
				, D3DPOOL_MANAGED
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				,0
				,&tInfo
				, NULL
				,(LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}
	
		m_vecTextureInfo.push_back(tInfo);
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
	if (!m_bClone)
	{
		for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
		m_vecTexture.clear();
	}
	CComponent::Free();
}
