#pragma once
#include "CUI.h"
#include "Monster.h"

typedef enum class BossHP
{UI_FRAME, UI_GAUGE, UI_BACK, MONSTERHP_END}BOSSHP;

enum BOSSNAME
{SUNGOLLEM, SUNGOLLEM_REBIRTH, SILKWORM, BOSSNAME_END};

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
	void	Set_Name(BOSSNAME eType);
	
private:
	BOSSHP		m_eUIType;
	BOSSNAME	m_eBossType;

private:
	_vec3		m_vDefaultPos;
	_int		m_iMaxHP;
	_int		m_iCurHP;
	_bool		m_bShown = false;

public:
	static  CUI_BossHP* Create(LPDIRECT3DDEVICE9 pGraphicDev, BOSSHP eType);

private:
	virtual void		Free() override;
};

