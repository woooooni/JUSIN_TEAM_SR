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

private:
	HRESULT Ready_Component();
	void	Key_Input();
	void	Add_Item(CItem* pItem)
	{
		m_ItemList.push_back(pItem);
	}
	void	Set_Item(); // Item setting

private:
	_bool				m_bShown = FALSE;
	list<CItem*>		m_ItemList;		   // 아이템 리스트

public:
	static  CUI_Shop* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

