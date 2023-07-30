#pragma once
#include "GameObject.h"
#include "GolemPart.h"
#include "MonsterAim.h"
#include "UI_BossHP.h"

BEGIN(Engine)
	class CRcTex;
class CTransform;
class CCollider;

END

class CSunGollem : public Engine::CGameObject
{
	CLONE(CSunGollem)
public:
	enum BODYPARTS {
		HEAD=0, LOWERJAW, UPPERJAW, LEFTLEG, RIGHTLEG, LEFTARM0, LEFTARM1,
		LEFTARM2, RIGHTARM0,RIGHTARM1, RIGHTARM2, LEFTHAND0, 
		LEFTHAND1, LEFTHAND2,RIGHTHAND0, RIGHTHAND1, RIGHTHAND2,FACE,PARTSEND};

protected:
	explicit CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSunGollem(const CSunGollem& rhs);
	virtual ~CSunGollem();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);
private:
	virtual void Update_Idle(_float fTimeDelta);
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta);
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta);
	virtual void Update_Regen(_float fTimeDelta);
public:
	static CSunGollem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void Free() override;
	
	HRESULT Ready_Parts(void);

	void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);

private:
	void Create_Fist(bool _BummerFist, _int _iSrc);
	void Create_Wave(_vec3 vPos);
	void Create_Stone();
	void Create_Effect();
	void Create_Monkey();

public:
	void Set_Speed(_float _fSpeed) { m_fSpeed = _fSpeed; }
	_float Get_Speed() { return m_fSpeed; }

	SUNGOLEM_STATE Get_State() { return m_eState; }
	void Set_State(SUNGOLEM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }

	// BossHPBar 연동으로 인해 추가함 (Test)
	MONSTERSTAT Get_Stat() { return m_tStat; }
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	vector<CGolemPart*> Get_vecGolemPart() { return m_vecGolemPart; }
	void Set_Time(_float _fTime) { m_fTime = _fTime; }
	_float Get_Time() { return m_fTime; }
	void Set_Stop(_bool _bStop) { m_bStop = _bStop; }
	_bool Is_Exhale() { return m_bExhale; }
protected:
	_vec3 m_vPartPos[PARTSEND];
	_float m_fPartAngleArray[PARTSEND] = {};
private:
	_float			m_fSpeed = 5.f;
	_float			m_fHealth = 6.f;
	SUNGOLEM_STATE	m_eState;
	MONSTERSTAT		m_tStat;
	// Hp UI를 위한 변수 추가.
	CUI_BossHP* m_pUIBack;
	CUI_BossHP* m_pUIFrame;
	CUI_BossHP* m_pUIGauge;
	_int			m_iDirtyHp = 1;
	vector<CGolemPart*> m_vecGolemPart;
private:
CGolemPart* m_pParts[PARTSEND];
CMonsterAim* m_pMonsterAim;
	CGameObject* m_pTarget;
	_float m_fMoveTime;
	_bool m_bExhale = false;
	_bool m_bStop = false;
	_bool m_bJump = false;
	_vec3 m_vVerticalDir = { 0,0,0 };
	_vec3 m_vRandomPos[3] = {};
	_vec3 m_vTargetPos ={0.f,0.f ,0.f};
	_int m_iRand = 1;
	_int m_iActiveArm = 2;
	bool m_bSummon[3];
	bool m_bAttack[6];
	bool m_bLockon = false;
	bool m_bDirty = false;
	_int m_iIndex;
	_float m_fTime=0.f;

};