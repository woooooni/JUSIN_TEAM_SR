#pragma once
#include "CUI.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END;

class CUI_Notification : public CUI
{
private:
	CUI_Notification(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_Notification(const CUI_Notification& rhs);
	virtual ~CUI_Notification();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);

private:
	_float	m_fMaxWidth;
	_float	m_fMaxHeight;
	_float	m_fCurWidth;
	_float	m_fCurHeight;
	_float	m_fSpeed = 10.f;

public:
	static  CUI_Notification* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

