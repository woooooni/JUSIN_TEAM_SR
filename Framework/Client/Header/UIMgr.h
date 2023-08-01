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
#include "UI_Veil.h"
#include "UI_MapName.h"
#include "UI_Wallet.h"
#include "UI_NewQuest.h"
#include "UI_QuestList.h"

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

public:
	CUI_Dialog* Get_Dialog() { return m_pDialog; }
	CQuickSlot* Get_Slots() { return m_pQuickSlot; }
	CUI_Veil* Get_Veil() { return m_pVeil; }
	CUI_Shop* Get_ShopUI() { return m_pShop; }
	CInventoryUI* Get_InventoryUI() { return m_pInventory; }
	CUI_ShortCutKey* Get_ShortcutKey() { return m_pShortCutKey; }
	CUI_Wallet*	Get_Wallet() { return m_pWallet; }
	CUI_NewQuest* Get_NewQuestUI() { return m_pNewQuest; }
	CUI_QuestList* Get_QuestList() { return m_pQuestList; }

	_bool	Get_ShopUpdate() { return m_bUpdateUI; }

private:
	CUI_Wallet*		 m_pWallet = nullptr;
	CUI_Dialog*		 m_pDialog = nullptr;
	CUI_HPBar*		 m_pHpBar= nullptr;
	CUI_Shop*		 m_pShop = nullptr;
	CUI_NewItem*	 m_pItemWindow = nullptr;
	CUI_Veil*		 m_pVeil = nullptr;
	CUI_NewQuest*	 m_pNewQuest = nullptr;
	CUI_QuestList*	 m_pQuestList = nullptr;
	
	CInventoryUI*	 m_pInventory = nullptr;

	CUI_ShortCutKey* m_pShortCutKey = nullptr;
	CQuickSlot*		 m_pQuickSlot = nullptr;
	CUI_BossHP*		 m_pBossHpBar = nullptr;

	_bool			m_bUpdateUI;

private:
	vector<CUI*>	m_vecIcon;

public:
	virtual void Free() override;
};

