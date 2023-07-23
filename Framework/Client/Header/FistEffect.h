#pragma once
#include "Effect.h"



class CFistEffect :
	public CEffect
{

	CLONE(CFistEffect)

protected:
	explicit CFistEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFistEffect(const CFistEffect& rhs);
	virtual ~CFistEffect();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT	Add_Component(void);
public:
	static CFistEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};