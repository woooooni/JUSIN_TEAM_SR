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
	}
	CUI_Cursor*		Get_Cursor() { return m_pCursor; }

private:
	HRESULT Ready_Component();
	void	Key_Input();
	void	Set_Item(); // Item setting

private:
	_bool				m_bShown = TRUE;
	list<CItem*>		m_ItemList;		   // 아이템 리스트
	CUI_Cursor*			m_pCursor = nullptr;
	vector<CUI*>		m_vecShopIcon;

public:
	static  CUI_Shop* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

