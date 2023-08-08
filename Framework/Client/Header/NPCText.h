#pragma once
#include "CUI.h"
#include "Npc.h"


class CNpcText : public CUI
{
	CLONE(CNpcText)

protected:
	explicit CNpcText(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpcText(const CNpcText& rhs);
	virtual ~CNpcText();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	virtual void		Free() override;

};