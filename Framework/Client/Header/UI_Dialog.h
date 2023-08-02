#pragma once
#include "CUI.h"
#include "Quest.h"

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
	void Set_Quest(CQuest* pQuest);
	void Set_DescTime(_float _fTime) { m_fDescTime = _fTime; }
	void Set_Name(const wstring& _strName) { m_strName = _strName; }

	CQuest* Get_CurrQuest() { return m_pQuest; }

	vector<wstring>& Get_StringVec() { return m_vecStrDesc; }

	const wstring& Get_Desc() { return m_strDesc; }

private:
	HRESULT Ready_Component();
	void Key_Input();
	void Print_Next();

private:
	wstring m_strDesc;
	wstring m_strCurrDesc;
	wstring m_strName;
	
	_float m_fAccTime;
	_float m_fDescTime;

	

	vector<wstring> m_vecStrDesc;
	_uint m_iStringIdx;
	_uint m_iVectorIdx;

	CQuest* m_pQuest;

public:
	static  CUI_Dialog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

