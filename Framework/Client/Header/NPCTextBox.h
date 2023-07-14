#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CText;

END

class CNPCTextBox : public CUI
{
	CLONE(CNPCTextBox)

private:
	CNPCTextBox(LPDIRECT3DDEVICE9 pGraphicDev);
	CNPCTextBox(const CNPCTextBox& rhs);
	virtual ~CNPCTextBox();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Shown(bool _bShown) { m_bShown = _bShown; }
	bool	Get_Shown()				{ return m_bShown; }

private:
	bool	m_bShown = false;

public:
	static  CNPCTextBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

