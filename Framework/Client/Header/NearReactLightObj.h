#pragma once
#include "NearReactObj.h"
class CNearReactLightObj : CNearReactObj
{
protected:
	CNearReactLightObj(LPDIRECT3DDEVICE9 pGraphicDev);
	CNearReactLightObj(const CNearReactObj& rhs);

	virtual ~CNearReactLightObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static	CNearReactObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos = { 0, 0, 0 });

protected:
	virtual HRESULT			Ready_Component(void) override;

};

