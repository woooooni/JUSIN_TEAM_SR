#pragma once
#include "GameObject.h"
#include "MonsterAim.h"

BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemFist : public Engine::CGameObject
{
	CLONE(CGolemFist)

protected:
	explicit CGolemFist(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemFist(const CGolemFist& rhs);
	virtual ~CGolemFist();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);

public:

	void Set_Dirty(_bool _bDirty) { if (m_bDirty == _bDirty) return; m_bDirty = _bDirty; }
	void Set_Bummer(_bool _bBummer) { if (m_bBummer == _bBummer) return; m_bBummer = _bBummer; }

public:
	
	static CGolemFist* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_Atk(_int _iAtk) { m_iAtk = _iAtk; }

private:
	CMonsterAim* m_pMonsterAim;
	_bool	m_bBummer = false;
	_bool	m_bDirty = false;
	_int m_iAtk = 0;
protected:
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};