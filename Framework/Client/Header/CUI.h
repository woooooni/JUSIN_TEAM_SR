#pragma once
#include "GameObject.h"


class CUI : public CGameObject
{
	CLONE(CUI)

protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	virtual void Debug_Input();

public:
	const UI_INFO& Get_UI_Info() { return m_tInfo; }

protected:
	_matrix		m_matProj;
	_matrix		m_matView;
	UI_INFO		m_tInfo;


protected:
	virtual void Free() override;
};

