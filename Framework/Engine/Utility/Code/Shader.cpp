#include "..\..\Header\Shader.h"


CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev, COMPONENT_TYPE::COM_SHADER)

{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	if(nullptr != m_pEffect)
		m_pEffect->AddRef();
}

CShader::~CShader()
{
}

HRESULT CShader::Ready_Shader( const _tchar* pShaderFilePath)
{
	/* m_pEffect를 만들자 .*/	
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return S_OK;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pShaderFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pShaderFilePath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CShader Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CShader::Clone(void)
{
	return new CShader(*this);
}

void CShader::Free()
{
	CComponent::Free();

	Safe_Release(m_pEffect);
}
