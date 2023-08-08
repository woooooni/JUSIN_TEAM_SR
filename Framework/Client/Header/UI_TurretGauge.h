#pragma once
#include "CUI.h"
#include	"Turret.h"

class CUI_TurretGauge :
    public CUI
{
private:
	CUI_TurretGauge(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_TurretGauge(const CUI_TurretGauge& rhs);
	virtual ~CUI_TurretGauge();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT			Add_Component(void);
	CTurret* Get_Owner() { return m_pOwner; }
	void			Set_Owner(CTurret* _pOwner) { m_pOwner = _pOwner; }

private:
	CTurret* m_pOwner;
	CTexture* m_pBack;

	_float		m_fEnergy;

public:
	static  CUI_TurretGauge* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTurret* pOwner);

private:
	virtual void		Free() override;

};

