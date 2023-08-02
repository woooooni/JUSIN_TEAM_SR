#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END;

class CUI_MainLogo : public CUI
{
private:
	explicit CUI_MainLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_MainLogo(const CUI_MainLogo& rhs);
	virtual ~CUI_MainLogo();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT	Ready_Component(void);


private:
	_float m_fAlpha;
	_bool m_bIncreaseAlpha;
	

public:
	static  CUI_MainLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

