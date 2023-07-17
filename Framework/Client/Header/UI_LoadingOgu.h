#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END;

class CUI_LoadingOgu : public CUI
{
private:
	explicit CUI_LoadingOgu(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_LoadingOgu(const CUI_LoadingOgu& rhs);
	virtual ~CUI_LoadingOgu();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT	Ready_Component(void);

public:
	static  CUI_LoadingOgu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

