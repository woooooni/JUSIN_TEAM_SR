#pragma once
#include "Effect.h"
class CEffect_Dig : public CEffect
{
	CLONE(CEffect_Dig)
protected:
	explicit CEffect_Dig(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Dig(const CEffect& rhs);
	virtual ~CEffect_Dig();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Dig* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale, _float fTime);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:
	_float m_fAccTime;
	_float m_fEffectTime;

	_uint m_iAlpha;
};

