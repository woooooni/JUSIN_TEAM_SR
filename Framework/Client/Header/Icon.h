#pragma once
#include "CUI.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

enum ICONTYPE
{
	PLAYERHP_FRAME,
	PLAYERHP_BACK,

	HEART, KEYBOARD, QUEST,
	KEYBUTTON_L,
	KEYBUTTON_1, KEYBUTTON_2,
	KEYBUTTON_3, KEYBUTTON_4,

	ICONTYPE_END
};

struct tagIconInfo
{
	ICONTYPE		eType;
};

class CIcon : public CUI
{
	CLONE(CIcon)
private:
	CIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	CIcon(const CIcon& rhs);
	virtual ~CIcon();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			Set_Type(ICONTYPE eType);
	void			Key_Input();

private:
	tagIconInfo			m_tIconInfo;
	_bool				m_bShown = FALSE;

public:
	static  CIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, ICONTYPE eType);

private:
	virtual void		Free() override;
};

