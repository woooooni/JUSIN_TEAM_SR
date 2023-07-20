#pragma once
#include	"CUI.h"

class CInventoryUI;

class CInvenTabButton : public CUI
{
public:
	CLONE(CInvenTabButton);

protected:
	explicit CInvenTabButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInvenTabButton(const CInvenTabButton& rhs);
	virtual ~CInvenTabButton();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	static	CInvenTabButton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CInventoryUI* pOwner,const _vec3& pPos = { 0.f, 0.f, 0.f });


public:
	virtual void Free() override;

protected:
	CInventoryUI* m_pOwner;


};

