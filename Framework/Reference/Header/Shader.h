#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader :	public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	LPD3DXEFFECT	Get_Effect() { return m_pEffect; }
	
public:
	HRESULT	Ready_Shader(const _tchar* pShaderFilePath);
	

private:
	LPD3DXEFFECT m_pEffect = { nullptr };

public:
	static CShader*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pShaderFilePath);
	virtual CComponent*	Clone(void);
private:
	virtual void Free();

};

END