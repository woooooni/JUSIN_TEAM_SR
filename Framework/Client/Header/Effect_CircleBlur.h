#pragma once
#include "Effect.h"
class CEffect_CircleBlur : public CEffect
{
	CLONE(CEffect_CircleBlur)
protected:
	explicit CEffect_CircleBlur(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_CircleBlur(const CEffect& rhs);
	virtual ~CEffect_CircleBlur();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_CircleBlur* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	
	void			Set_Effect(_vec3& _vPos, _vec3& _vScale, D3DCOLORVALUE& vColor, D3DCOLORVALUE& vEmissive);

	void			Set_Color(D3DCOLORVALUE& vColor) 
	{
		m_iR = _int(vColor.r * 255.0f); 
		m_iG = _int(vColor.g * 255.0f);
		m_iB = _int(vColor.b * 255.0f);
		m_iAlpha = _int(vColor.a * 255.0f);
	}

	void			Set_Emissive(D3DCOLORVALUE& vEmissive)
	{
		m_tMaterial.Emissive = vEmissive;
	}

protected:
	virtual HRESULT	Add_Component(void);


	
protected:
	virtual void Free() override;

private:
	_float m_fAccTime;
	_float m_fEffectTime;

	_int m_iR;
	_int m_iG;
	_int m_iB;

	_vec3 m_vDir;
};

