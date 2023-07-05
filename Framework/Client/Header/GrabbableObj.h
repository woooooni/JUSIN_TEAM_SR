#pragma once
#include "Engine_Define.h"
#include "FieldObject.h"


class CGrabbableObj : public CFieldObject
{
	CGrabbableObj(LPDIRECT3DDEVICE9 pGraphicDev);
	CGrabbableObj(const CGrabbableObj& rhs);
	virtual ~CGrabbableObj();


public:
	virtual			void		Grap();
	virtual			void		UnGrap();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);

public:
	static		CGrabbableObj* Create(LPDIRECT3DDEVICE9 pDev, const _vec3 p_Pos = { 0, 0, 0 });

};

