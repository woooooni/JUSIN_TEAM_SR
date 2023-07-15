#pragma once
#include "Effect.h"

class CEffect_Leaf : public CEffect
{
	CLONE(CEffect_Leaf)

protected:
	explicit CEffect_Leaf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Leaf(const CEffect& rhs);
	virtual ~CEffect_Leaf();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	static CEffect_Leaf* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void			Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iCount);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:

};

