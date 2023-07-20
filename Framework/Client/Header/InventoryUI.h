#pragma once
#include "CUI.h"

class CInvenTabButton;

class CInventoryUI :    public CUI
{
	CLONE(CInventoryUI);

protected:
	explicit CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventoryUI(const CInventoryUI& rhs);
	virtual ~CInventoryUI();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	static	CInventoryUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	vector<CInvenTabButton*> m_vecTabButton;
	_uint		m_iCurIdx;

protected:
	virtual void Free() override;


};

