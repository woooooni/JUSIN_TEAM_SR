#pragma once
#include "Effect.h"
class CEffect_Smoke : public CEffect
{
	CLONE(CEffect_Smoke)
protected:
	explicit CEffect_Smoke(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Smoke(const CEffect& rhs);
	virtual ~CEffect_Smoke();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Smoke* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iR = 255, _uint _iG = 255, _uint _iB = 255);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;

private:
	_float m_fAccTime;
	_float m_fEffectTime;

	_uint m_iAlpha;
	_uint m_iR;
	_uint m_iG;
	_uint m_iB;

	_vec3 m_vScale;
	_float m_fScale;

	_vec3 m_vDir;
};

