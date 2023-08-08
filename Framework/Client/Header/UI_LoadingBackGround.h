#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END;

class CUI_LoadingBackGround : public CUI
{
private:
	explicit CUI_LoadingBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_LoadingBackGround(const CUI_LoadingBackGround& rhs);
	virtual ~CUI_LoadingBackGround();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT	Ready_Component(void);

public:
	static  CUI_LoadingBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

