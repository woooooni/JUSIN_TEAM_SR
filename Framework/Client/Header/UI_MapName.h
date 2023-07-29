#pragma once
#include "CUI.h"

class CUI_MapName : public CUI
{
	CLONE(CUI_MapName)

private:
	CUI_MapName(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_MapName(const CUI_MapName& rhs);
	virtual ~CUI_MapName();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();
	void	Set_Scene(SCENE_TYPE eType) { m_eSceneType = eType; }

private:
	_float		m_fAlpha;
	_float		m_fMaxWidth;
	_float		m_fSusTime;
	_bool		m_bIsMax = false;
	SCENE_TYPE	m_eSceneType;

public:
	static  CUI_MapName* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE eType);

private:
	virtual void		Free() override;
};

