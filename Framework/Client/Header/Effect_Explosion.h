#pragma once
#include "Effect.h"
class CEffect_Explosion : public CEffect
{
	CLONE(CEffect_Explosion)
protected:
	explicit CEffect_Explosion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Explosion(const CEffect& rhs);
	virtual ~CEffect_Explosion();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Explosion* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;
};

