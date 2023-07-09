#pragma once
#include "GameObject.h"
class CTile :
    public CGameObject
{
	CLONE(CTile)

protected:
	explicit CTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTile(const CTile& rhs);
	virtual ~CTile();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();

public:
	static CTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;
};

