#pragma once
#include "CUI.h"
class CUI_Ending :   public CUI
{

public:
	CLONE(CUI_Ending);

protected:
	explicit CUI_Ending(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Ending(const CUI_Ending& rhs);
	virtual ~CUI_Ending();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	static CUI_Ending* Create(LPDIRECT3DDEVICE9 p_GraphicDev);

protected:
	vector<wstring>		m_vecNameTags;
	vector<float>		m_vecPosition;

};

