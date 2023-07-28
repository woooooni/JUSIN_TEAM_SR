#pragma once
#include "Effect.h"
class CEffect_LightningGround : public CEffect
{
	CLONE(CEffect_LightningGround)
protected:
	explicit CEffect_LightningGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_LightningGround(const CEffect& rhs);
	virtual ~CEffect_LightningGround();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;


	static CEffect_LightningGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;
};

