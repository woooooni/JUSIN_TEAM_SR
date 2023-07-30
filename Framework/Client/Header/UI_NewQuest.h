#pragma once
#include "CUI.h"
#include "UI_QuestIcon.h"
#include "UI_Notification.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END;

class CUI_NewQuest : public CUI
{
	CLONE(CUI_NewQuest)

private:
	CUI_NewQuest(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_NewQuest(const CUI_NewQuest& rhs);
	virtual ~CUI_NewQuest();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void Set_Title(const wstring& _strTitle) { m_strTitle = _strTitle; }
	void Set_Desc(const wstring& _strDesc) { m_strDesc = _strDesc; }



private:
	void	Key_Input();

private:
	_float	m_fMaxWidth;
	_float	m_fMaxHeight;
	_float	m_fCurWidth;
	_float	m_fCurHeight;
	_float	m_fSpeed = 10.f;
	_bool	m_bShown = false;

private:
	CUI_QuestIcon* m_pCloseKey = nullptr;
	CUI_QuestIcon* m_pTitleBox = nullptr;
	CUI_QuestIcon* m_pContentsBox = nullptr;
	CUI_QuestIcon* m_pExclamIcon = nullptr;
	CUI_Notification* m_pWindow = nullptr;

private:
	wstring m_strTitle;
	wstring m_strDesc;

public:
	static  CUI_NewQuest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

