#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CText;

END

class CNPCText : public CUI
{
	CLONE(CNPCText)
private:
	CNPCText(LPDIRECT3DDEVICE9 pGraphicDev);
	CNPCText(const CNPCText& rhs);
	virtual ~CNPCText();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNPCText*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

