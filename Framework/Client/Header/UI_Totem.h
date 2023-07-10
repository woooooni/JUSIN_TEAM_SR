#pragma once
#include "CUI.h"
#include "Player.h"

typedef enum class TotemType
{
	CURR, MAX, DECO,
	TOTEM_END

}TOTEMTYPE;

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Totem : public CUI
{
private:
	CUI_Totem(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_Totem(const CUI_Totem& rhs);
	virtual ~CUI_Totem();

public:
	HRESULT Add_Component(void);
	void	Set_Type(TOTEMTYPE eType);

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	TOTEMTYPE	m_eTotemType;
	_uint		m_iMaximum = 3;
	_uint		m_iCurrValue = 2;

public:
	static  CUI_Totem* Create(LPDIRECT3DDEVICE9 pGraphicDev, TOTEMTYPE eType);

private:
	virtual void		Free() override;
};

