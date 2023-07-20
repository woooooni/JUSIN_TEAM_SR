#pragma once
#include "CUI.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_HPBar : public CUI
{
private:
	CUI_HPBar(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_HPBar(const CUI_HPBar& rhs);
	virtual ~CUI_HPBar();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	_vec3	m_vDefaultPos;
	_int	m_iMaxHP;
	_int	m_iHP;

public:
	static  CUI_HPBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

