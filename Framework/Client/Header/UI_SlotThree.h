#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_SlotThree : public CUI
{
	CLONE(CUI_SlotThree)

private:
	explicit CUI_SlotThree(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SlotThree(const CUI_SlotThree& rhs);
	virtual ~CUI_SlotThree();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CUI_SlotThree* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

