#pragma once
#include "CUI.h"

class CUI_Dialog : public CUI
{
	CLONE(CUI_Dialog)

private:
	explicit CUI_Dialog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Dialog(const CUI_Dialog& rhs);
	virtual ~CUI_Dialog();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Shown(bool _bShown) { m_bShown = _bShown; }
	_bool	Get_Shown() { return m_bShown; }

	void Set_DescTime(_float _fTime) { m_fDescTime = _fTime; }
	void Set_Desc(const wstring& _strDesc) { m_strCurrDesc = L""; m_strDesc = _strDesc; m_fAccTime = 0.f; m_iIdx = 0; }

	void Set_Name(const wstring& _strName) { m_strName = _strName; }

private:
	HRESULT Ready_Component();

private:
	_bool	m_bShown = false;

	wstring m_strDesc;
	wstring m_strCurrDesc;
	wstring m_strName;
	
	_float m_fAccTime;
	_float m_fDescTime;

	_uint m_iIdx;


public:
	static  CUI_Dialog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

