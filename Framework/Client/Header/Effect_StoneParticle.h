#pragma once
#include "Effect.h"
class CEffect_StoneParticle : public CEffect
{
	CLONE(CEffect_StoneParticle)

protected:
	explicit CEffect_StoneParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_StoneParticle(const CEffect& rhs);
	virtual ~CEffect_StoneParticle();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;


	static CEffect_StoneParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void			Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iCount);

protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:

};

