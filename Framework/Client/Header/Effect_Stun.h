#pragma once
#include "Effect.h"
class CEffect_Stun : public CEffect
{
	CLONE(CEffect_Stun)
protected:
	explicit CEffect_Stun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Stun(const CEffect& rhs);
	virtual ~CEffect_Stun();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Stun* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void			Get_Effect(CGameObject* _pObj, _vec3& _vOffSet, _vec3& _vScale, _float* fTime);


protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:
	_float* m_fEffectTime;

	CGameObject* m_pOwner = nullptr;
	_vec3 m_vOffSet;
};

