#pragma once
#include "CUI.h"

class CNpc;
class CUI_ContinueMark : public CUI
{
private:
	explicit CUI_ContinueMark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_ContinueMark(const CUI_ContinueMark& rhs);
	virtual ~CUI_ContinueMark();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Owner(CNpc* _pOwner) { m_pOwner = _pOwner; }

public:
	CNpc* m_pOwner = nullptr;

public:
	static  CUI_ContinueMark* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

