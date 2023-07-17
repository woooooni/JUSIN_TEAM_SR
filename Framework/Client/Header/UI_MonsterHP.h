#pragma once
#include "CUI.h"


// Back, Frame, HP

typedef enum class MonsterHP
{
	UI_FRAME,
	UI_GAUGE,
	UI_BACK,

	MONSTERHP_END

}MONSTERHP;

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;


END;
class CMonster;

class CUI_MonsterHP : public CUI
{
private:
	CUI_MonsterHP(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_MonsterHP(const CUI_MonsterHP& rhs);
	virtual ~CUI_MonsterHP();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Type(MONSTERHP eType);
	CMonster* Get_Owner() { return m_pOwner; }
	void			Set_Owner(CMonster* _pOwner) { m_pOwner = _pOwner; }

private:
	MONSTERHP	m_eUIType;
	_int		m_iMaxHP; // 몬스터 최대 체력
	_int		m_iHP;	  // 몬스터 현재 체력
	CMonster*	m_pOwner;
public:
	static  CUI_MonsterHP* Create(LPDIRECT3DDEVICE9 pGraphicDev, MONSTERHP eType);

private:
	virtual void		Free() override;
};

