#pragma once
#include "CUI.h"
#include "UI_Cursor.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

// 구조체 이름 바꾸기

typedef enum class UI_ShopItem
{
	UISHOP_BRANCH,
	UISHOP_CLOTH,
	UISHOP_LEAF,
	UISHOP_BRANCH_INFO,
	UISHOP_CLOTH_INFO,
	UISHOP_LEAF_INFO,

	SHOPKEY_L,
	SHOP_WALLET, SHOP_PRICETAG,
	SHOP_TEXTBOX, SHOP_IMGBOX,
	SHOP_VERLINE, SHOP_HORLINE,
	SHOPITEM_END

}SHOPITEMTYPE;

struct tagShopItemInfo
{
	SHOPITEMTYPE	eType;
	_int			iPrice;
};

class CUI_ItemInfo : public CUI
{
	CLONE(CUI_ItemInfo)

private:
	explicit CUI_ItemInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_ItemInfo(const CUI_ItemInfo& rhs);
	virtual ~CUI_ItemInfo();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			Set_Type(SHOPITEMTYPE eType);
	void			Key_Input();

private:
	tagShopItemInfo		m_tItemInfo;
	_vec3				m_vDefaultPos;
	_bool				m_bShown = false;
	_bool				m_bCursorMove = false;
	_uint				m_iCursorX = 0;
	_uint				m_iCursorY = 0;

public:
	static  CUI_ItemInfo* Create(LPDIRECT3DDEVICE9 pGraphicDev, SHOPITEMTYPE eType);

private:
	virtual void		Free() override;
};

