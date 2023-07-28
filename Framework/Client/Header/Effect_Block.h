#pragma once
#include "Effect.h"
class CEffect_Block : public CEffect
{
	CLONE(CEffect_Block)
protected:
	explicit CEffect_Block(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Block(const CEffect& rhs);
	virtual ~CEffect_Block();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;


	static CEffect_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;
};

