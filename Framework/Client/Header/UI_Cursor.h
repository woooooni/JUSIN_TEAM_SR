#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUI_Cursor : public CUI
{
private:
	explicit CUI_Cursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Cursor(const CUI_Cursor& rhs);
	virtual ~CUI_Cursor();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();
	void	Key_Input();

private:
	_bool	m_bShown = FALSE;
	_vec3	m_vDefaultPos;
	_uint	m_iCursorX = 0; // X축으로 이동하는 Cursor
	_uint	m_iCursorY = 0; // Y축으로 이동하는 Cursor

public:
	static  CUI_Cursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

