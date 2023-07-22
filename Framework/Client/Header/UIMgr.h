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

	HRESULT		Change_UI(SCENE_TYPE pUI)
	{
		switch (pUI)
		{
		case Engine::SCENE_TYPE::LOADING:
			break;
		case Engine::SCENE_TYPE::TEST:
			break;
		case Engine::SCENE_TYPE::LOGO:
			break;
		case Engine::SCENE_TYPE::TUTORIAL_VILLAGE:
			break;
		case Engine::SCENE_TYPE::MONKEY_FOREST1:
			break;
		case Engine::SCENE_TYPE::MONKEY_FOREST2:
			break;
		case Engine::SCENE_TYPE::MONKEY_VILLAGE:
			break;
		case Engine::SCENE_TYPE::SUNGOLEM_CAVE1:
			break;
		case Engine::SCENE_TYPE::MOON_FOREST1:
			break;
		case Engine::SCENE_TYPE::TOOL:
			break;
		case Engine::SCENE_TYPE::SCENE_END:
			break;
		default:
			break;
		}
	}

private:
	CUI_Dialog*		 m_pDialog = nullptr;
	CUI_HPBar*		 m_pHpBar= nullptr;
	CUI_Shop*		 m_pShop = nullptr;

	CUI* m_pCurrentUI = nullptr;

	CUI_ShortCutKey* m_pShortCutKey = nullptr;
	CQuickSlot*		 m_pQuickSlot = nullptr;
	CUI_BossHP*		 m_pBossHpBar = nullptr;

private:
	vector<CUI*>	m_vecIcon;

public:
	virtual void Free() override;
};

