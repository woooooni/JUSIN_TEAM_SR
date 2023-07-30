#pragma once
#include "Effect.h"
class CEffect_Shadow : public CEffect
{
	CLONE(CEffect_Shadow)

protected:
	explicit CEffect_Shadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Shadow(const CEffect& rhs);
	virtual ~CEffect_Shadow();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Shadow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void			Set_Shadow(CGameObject* _pObj, _vec3& _vScale);
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;


private:
	CGameObject* m_pOwner;

	_vec3 m_vScale;

	void	Set_Scale(_vec3 _vScale);
};

