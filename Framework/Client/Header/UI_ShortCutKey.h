#pragma once
#include "CUI.h"

class CUI_ShortCutKey : public CUI
{
private:
	CUI_ShortCutKey(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_ShortCutKey(const CUI_ShortCutKey& rhs);
	virtual ~CUI_ShortCutKey();

public:
	void	Set_Shown(bool _bShown) { m_bShown = _bShown; }
	bool	Get_Shown()				{ return m_bShown; }

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	bool		m_bShown = false;

public:
	static  CUI_ShortCutKey* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

