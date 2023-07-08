#pragma once
#include "GameObject.h"


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
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);


public:

	SUNGOLEM_STATE Get_State() { return m_eState; }
	void Set_State(SUNGOLEM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }
	void Set_Dirty(_bool _bDirty) { if (m_bDirty == _bDirty) return; m_bDirty = _bDirty; }
	void Set_Bummer(_bool _bBummer) { if (m_bBummer == _bBummer) return; m_bBummer = _bBummer; }

public:
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }

	static CGolemFist* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void Set_ArmNum(_int _iNum) { m_iArmNum = _iNum; }

private:
	SUNGOLEM_STATE	m_eState;
	bool m_bBreath = false;
	_int m_iArmNum = 0;
	_bool	m_bBummer = false;
	_bool	m_bDirty = false;

protected:
	CGameObject* m_pTarget;
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};