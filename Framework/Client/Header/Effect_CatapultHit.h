#pragma once
#include "Effect.h"
class CEffect_CatapultHit : public CEffect
{
	CLONE(CEffect_CatapultHit)
protected:
	explicit CEffect_CatapultHit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_CatapultHit(const CEffect& rhs);
	virtual ~CEffect_CatapultHit();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	static CEffect_CatapultHit* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;
};

