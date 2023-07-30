#pragma once
#include "CUI.h"

class CUI_Wallet : public CUI
{
private:
	CUI_Wallet(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_Wallet(const CUI_Wallet& rhs);
	virtual ~CUI_Wallet();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_WalletAlpha(_uint _iAlpha) { m_iAlpha = _iAlpha; }

private:
	_uint	m_iPlayerMoney = 0;
	_uint	m_iAlpha = 255;
	_float	m_fShowingTime = 0.f;
	_bool	m_bShown = true;

public:
	static  CUI_Wallet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

