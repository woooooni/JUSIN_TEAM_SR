#pragma once

#include "GameObject.h"
#include "UI_MonsterHP.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eObjId);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);
	
public:
	void	Set_Speed(_float _fSpeed)	{ m_fSpeed = _fSpeed; }
	_float  Get_Speed()					{ return m_fSpeed; }

	MONSTER_STATE	Get_State()			{ return m_eState; }
	void			Set_State(MONSTER_STATE _eState)
	{
		if (m_eState == _eState)
			return;
		m_eState = _eState;
	}

	MONSTERSTAT Get_Stat()				{ return m_tStat; }

public:
	void		 Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	virtual void Trace(_float fTimeDelta) PURE;

	void		Set_Stun(_float _fStunTime);

	void Set_Summoned_By_Prist(_bool _bSummonedByPrist) { m_bSummonedByPrist = _bSummonedByPrist; }
	void Set_DefenceMode(CGameObject* _pTarget);

public:
	virtual void Update_Idle(_float fTimeDelta)		PURE;
	virtual void Update_Regen(_float fTimeDelta)	PURE;
	virtual void Update_Move(_float fTimeDelta)		PURE;
	virtual void Update_Attack(_float fTimeDelta)	PURE;
	virtual void Update_Die(_float fTimeDelta)		PURE;
	virtual void Update_Stun(_float fTimeDelta);
	virtual void Update_DefenceMode(_float fTimeDelta);


private:
	_float			m_fSpeed = 5.f;
	MONSTER_STATE	m_eState;

protected:
	CGameObject*	m_pTarget;
	_vec3			m_vTargetPos;

	MONSTERSTAT		m_tStat;
	_bool m_bPushable = true;
	_bool m_bSummonedByPrist = false;
	CUI_MonsterHP*	m_pUIBack;
	CUI_MonsterHP*	m_pUIFrame;
	CUI_MonsterHP*	m_pUIGauge;

	_vec3 m_vLook = {0.f,0.f,-1.f};
	MONSTER_STATE m_ePreviousState;
	OBJ_DIR m_eDir;

	_float m_fStunTime;

	_bool m_bDefenceMode = false;
protected:
	virtual void Free() override;
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)	;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)	override;
	void Push_Me(CCollider* other);
	void On_Death();
};

