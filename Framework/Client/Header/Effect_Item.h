#pragma once
#include "Effect.h"
class CEffect_Item : public CEffect
{
	CLONE(CEffect_Item)
protected:
	explicit CEffect_Item(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Item(const CEffect& rhs);
	virtual ~CEffect_Item();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	static CEffect_Item* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(_vec3& _vPos, ITEM_CODE _eItemCode);
	void			End_Effect() { m_iTurn = 2; }
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:
	_int m_iTurn;
	_vec3 m_vScale;
};

