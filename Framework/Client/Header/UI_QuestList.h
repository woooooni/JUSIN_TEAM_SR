#pragma once
#include "CUI.h"
#include "Quest.h"

class CUI_QuestList : public CUI
{
	CLONE(CUI_QuestList)

private:
	CUI_QuestList(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_QuestList(const CUI_QuestList& rhs);
	virtual ~CUI_QuestList();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Title(const wstring& _strTitle) { m_strTitle = _strTitle; }
	_bool	Get_Shown() { return m_bShown; }
	void	Set_Shown(_bool _bShown) { m_bShown = _bShown; }

private:
	HRESULT Ready_Component();

private:
	wstring m_strTitle; // Äù½ºÆ® ³»¿ë
	_bool	m_bShown = false;

public:
	static  CUI_QuestList* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

