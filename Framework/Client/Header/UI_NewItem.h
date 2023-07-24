#pragma once
#include "CUI.h"
#include "UI_QuestIcon.h"
#include "UI_Notification.h"

class CItem;

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END;

class CUI_NewItem : public CUI
{
	CLONE(CUI_NewItem)

private:
	CUI_NewItem(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_NewItem(const CUI_NewItem& rhs);
	virtual ~CUI_NewItem();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Get_ItemInfo(ITEM_CODE _eCodeType);

private:
	HRESULT	Add_Component(void);
	void	Key_Input();
	void	Set_Item(ITEM_CODE _eCodeType);

private:
	_float	m_fMaxWidth;
	_float	m_fCurWidth;
	_float	m_bShown = false;
	wstring m_strItemInfo; // Item ��� ����
	wstring m_strItemName; // Item �̸�

private:
	CUI_QuestIcon* m_pCloseKey = nullptr; // �ݱ��ư
	CUI_QuestIcon* m_pFrame = nullptr; // ������ Frame
	CUI_Notification* m_pWindow = nullptr; // â

public:
	static  CUI_NewItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};