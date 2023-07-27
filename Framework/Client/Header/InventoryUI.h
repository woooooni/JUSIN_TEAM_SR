#pragma once
#include "CUI.h"

class CInvenTabButton;

class CInventoryUI :    public CUI
{
	CLONE(CInventoryUI);

protected:
	explicit CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventoryUI(const CInventoryUI& rhs);
	virtual ~CInventoryUI();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	static	CInventoryUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void		Set_ButClicked(CInvenTabButton* pInv);
	void		Register_QuickSlot();

	void		Reset_Index()
	{
		m_iCurItemIdx = 0;
		m_iCurPageIdx = 0;
		m_bIsRenderCurs = false;
	}

protected:
	vector<CInvenTabButton*> m_vecTabButton;
	_uint		m_iCurPageIdx;
	_uint		m_iCurItemIdx;

	_bool		m_bIsRenderCurs;

	CTexture* m_pCursurTex;
	CTexture* m_pUseBtnTex;

	UI_INFO		m_tCursorInfo;
	UI_INFO		m_tBtnInfo;
	UI_INFO		m_tExplainInfo;

	


protected:
	virtual void Free() override;


};

