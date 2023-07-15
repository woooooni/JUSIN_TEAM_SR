#pragma once
#include "CUI.h"
#include "Monster.h"
#include "SunGollem.h"

typedef enum class BossHP
{
	UI_FRAME,
	UI_GAUGE,
	UI_BACK,

	MONSTERHP_END

}BOSSHP;

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END;

class CUI_BossHP : public CUI
{
private:
	CUI_BossHP(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_BossHP(const CUI_BossHP& rhs);
	virtual ~CUI_BossHP();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Type(BOSSHP eType);

private:
	BOSSHP		m_eUIType;     // Frame, Gauge, Back 구분
	_vec3		m_vDefaultPos; // UI가 띄워지는 기준점
	_int		m_iMaxHP;	   // 보스 최대 체력
	_int		m_iCurHP;	   // 보스 현재 체력
	_bool		m_bShown = true; // UI가 보여지고 있는가?

public:
	static  CUI_BossHP* Create(LPDIRECT3DDEVICE9 pGraphicDev, BOSSHP eType);

private:
	virtual void		Free() override;
};

