#pragma once
#include "NearReactObj.h"
class CNearReactMushroom :    public CGameObject
{
	CLONE(CNearReactMushroom)

protected:
	CNearReactMushroom(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	CNearReactMushroom(const CNearReactMushroom& rhs);

	virtual ~CNearReactMushroom();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static	CNearReactMushroom* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos = { 0, 0, 0 });

protected:
	virtual HRESULT			Ready_Component(void);

	CAnimator* m_pSubAnimator;
	_bool		m_bHide = false;

};

