#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CAnimator;

END

class CUIPages : public CUI
{
	CLONE(CUIPages)

private:
    CUIPages(LPDIRECT3DDEVICE9 pGraphicDev);
    CUIPages(const CUIPages& rhs);
    virtual ~CUIPages();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CUIPages*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

