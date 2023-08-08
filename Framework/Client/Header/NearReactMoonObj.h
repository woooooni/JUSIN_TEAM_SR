#pragma once
#include "NearReactObj.h"
class CNearReactMoonObj :
    public CNearReactObj
{
protected:
	CNearReactMoonObj(LPDIRECT3DDEVICE9 pGraphicDev);
	CNearReactMoonObj(const CNearReactMoonObj& rhs);

	virtual ~CNearReactMoonObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static	CNearReactMoonObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos = { 0, 0, 0 });

protected:
	virtual HRESULT			Ready_Component(void) override;

	bool			m_bIsClose = false;



};

