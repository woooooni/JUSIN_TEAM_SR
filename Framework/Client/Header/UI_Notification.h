#pragma once
#include "CUI.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END;

class CUI_Notification : public CUI
{
private:
	CUI_Notification(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_Notification(const CUI_Notification& rhs);
	virtual ~CUI_Notification();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Ratio(_float fWidthRatio, _float fHeightRatio);
	void	Set_Owner(CUI* _pOwner) { m_pOwner = _pOwner; }
	_float	Get_CurWidth() { return m_fCurWidth; }
	_float	Get_MaxWidth() { return m_fMaxWidth; }

private:
	_float	m_fHeightRatio;
	_float	m_fWidthRatio;
	_float	m_fMaxWidth;
	_float	m_fMaxHeight;
	_float	m_fCurWidth;
	_float	m_fCurHeight;
	_float	m_fSpeed = 10.f;
	CUI*	m_pOwner = nullptr;

public:
	static  CUI_Notification* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_float fWidthRatio = 1.f, _float fHeightRatio = 1.f);

private:
	virtual void		Free() override;
};

