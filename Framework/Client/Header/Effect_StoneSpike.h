#pragma once
#include "Bullet.h"



class CEffect_StoneSpike :
	public CBullet
{

	CLONE(CEffect_StoneSpike)

protected:
	explicit CEffect_StoneSpike(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_StoneSpike(const CEffect_StoneSpike& rhs);
	virtual ~CEffect_StoneSpike();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT	Add_Component(void);
public:
	static CEffect_StoneSpike* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};