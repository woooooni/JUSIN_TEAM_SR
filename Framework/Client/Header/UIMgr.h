#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "QuickSlot.h"
#include "UI_Dialog.h"
#include "UI_HPBar.h"
#include "UI_Shop.h"
#include "UI_ShortCutKey.h"
#include "Icon.h"
#include "UI_BossHP.h"
#include "InventoryUI.h"
#include "UI_NewItem.h"

class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	HRESULT		Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev);
	void		Update_UIMgr(const _float& fTimeDelta);
	void		Late_Update_UIMgr();
	void		Render_UIMgr();

public:
	HRESULT		Add_Icon(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Add_Frame(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Add_Window(ITEM_CODE eType);

private:
	CUI_Dialog*		 m_pDialog = nullptr;
	CUI_HPBar*		 m_pHpBar= nullptr;
	CUI_Shop*		 m_pShop = nullptr;
	CUI_NewItem* m_pItemWindow = nullptr;

	CUI* m_pCurrentUI = nullptr;

	CUI_ShortCutKey* m_pShortCutKey = nullptr;
	CQuickSlot*		 m_pQuickSlot = nullptr;
	CUI_BossHP*		 m_pBossHpBar = nullptr;

	_bool			m_bUpdateUI;

private:
	vector<CUI*>	m_vecIcon;

public:
	virtual void Free() override;
};

