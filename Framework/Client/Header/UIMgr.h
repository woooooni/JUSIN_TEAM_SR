#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "QuickSlot.h"
#include "UI_Dialog.h"
#include "UI_HPBar.h"
#include "UI_Shop.h"
#include "UI_ShortCutKey.h"
#include "Icon.h"

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
	void		Set_PlayerInfo(); // 체력을 받아와서 씬 전환때 적용시킴
	HRESULT		Set_UI(SCENE_TYPE eType);

private:
	CUI_Dialog*		 m_pDialog = nullptr;
	CUI_HPBar*		 m_pHpBar= nullptr;
	CUI_Shop*		 m_pShop = nullptr;
	CUI_ShortCutKey* m_pShortCutKey = nullptr;
	CQuickSlot*		 m_pQuickSlot = nullptr;

private:
	vector<CUI*>	m_vecIcon;

public:
	virtual void Free() override;
};

