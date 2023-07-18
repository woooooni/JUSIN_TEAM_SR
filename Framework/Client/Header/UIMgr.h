#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "UI_Dialog.h"
#include "UI_HPBar.h"
#include "UI_Shop.h"
#include "UI_ShortCutKey.h"
class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)

private:
	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	HRESULT Ready_UIMgr(LPDIRECT3DDEVICE9 _pGraphicDev);
	void Update_UIMgr(const _float& fTimeDelta);
	void Late_Update_UIMgr();
	void Render_UIMgr();


private:
	CUI_Dialog* m_pDialog = nullptr;
	CUI_HPBar* m_pHpBar= nullptr;
	CUI_Shop* m_pShop = nullptr;
	CUI_ShortCutKey* m_pShortCutKey = nullptr;

public:
	virtual void Free() override;
};

