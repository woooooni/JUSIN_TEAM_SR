#pragma once
#include "Effect.h"
class CEffect_DieSmoke : public CEffect
{
	CLONE(CEffect_DieSmoke)
protected:
	explicit CEffect_DieSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_DieSmoke(const CEffect& rhs);
	virtual ~CEffect_DieSmoke();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	static CEffect_DieSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;
};

