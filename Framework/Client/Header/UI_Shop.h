#pragma once
#include "CUI.h"
#include "Item.h"
#include "UI_ItemInfo.h"
#include "UI_Cursor.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUI_Shop : public CUI
{
private:
	explicit CUI_Shop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Shop(const CUI_Shop& rhs);
	virtual ~CUI_Shop();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Add_Item(CItem* pItem, const SHOPITEMTYPE& pType)
	{
		dynamic_cast<CUI_ItemInfo*>(m_vecShopIcon[(_uint)pType])->Set_Item(pItem);
		dynamic_cast<CUI_ItemInfo*>(m_vecShopIcon[(_uint)pType + 1])->Set_Item(pItem);

	}
	CUI_Cursor*		Get_Cursor() { return m_pCursor; }
	vector<CUI*>	Get_ShopUI() { return m_vecShopIcon; }
	_bool			Get_Shown() { return m_bShown; }
	void	Remove_Item(SHOPITEMTYPE pType);
	void	Set_ItemInfo(ITEM_CODE _eCode, _uint _iPrice);

private:
	HRESULT Ready_Component();
	void	Key_Input();

private:
	_bool				m_bShown = false;
	list<CItem*>		m_ItemList;
	CUI_Cursor*			m_pCursor = nullptr;
	vector<CUI*>		m_vecShopIcon;
	vector<_bool>		m_vecShown;

	_uint				m_iCursorX = 0;
	_uint				m_iCursorY = 0;
	_uint				m_iItemPrice = 0;
	wstring				m_strItemName = L"";
	wstring				m_strItemInfo = L"";

public:
	static  CUI_Shop* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

