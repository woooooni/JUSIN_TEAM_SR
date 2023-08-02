#pragma once
#include "Monster.h"
#include "UI_BossHP.h"

BEGIN(Engine)
class RcCol;
END

class CSilkWorm :
    public CGameObject
{
    CLONE(CSilkWorm)
private:
    explicit CSilkWorm(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSilkWorm(const CSilkWorm& rhs);
    virtual ~CSilkWorm();
public:
     void Update_Idle(_float fTimeDelta);
     void Update_Die(_float fTimeDelta);
     void Update_Regen(_float fTimeDelta);
     void Update_Ready(_float fTimeDelta);
     void Update_Attack(_float fTimeDelta);
     void Update_Down(_float fTimeDelta);
    
public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)		override;
    virtual void Render_Object(void) override;
    virtual void	LateUpdate_Object(void)						override;

    HRESULT	Add_Component(void);
private:
    void Trace(_float fTimeDelta);
     void Create_Effect(_vec3 vPos);
     void Shoot_BugBall();
     void Shoot_PlantBallFirst();
     void Shoot_PlantBallSecond();
     void Shoot_Doppel();
     void Create_Line();
     void Create_NueFlower();
    void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);


    // BossHPBar 연동으로 인해 추가함 (Test)
   
public: 
    MONSTERSTAT Get_Stat() { return m_tStat; }
    static  CSilkWorm* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    SILKWORM_STATE Get_State() { return m_eState; }
    void Set_State(SILKWORM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }

private:
    void Generate_MotionTrail(_float fTImeDelta);

private:
    _float m_fMoveTime;
    _float m_fEffectCoolTime =0.f;
    _vec3 m_vDst = { 0.f,0.f,0.f };
    _vec3 m_vDir = { 0.f,0.f,0.f }; 
    _bool m_bShoot = true;
    _int m_iShootState=0;
    _bool m_bPosAccupied[8] = {};
    _float m_fAttackTerm;
    CGameObject* m_pNueFlower[6] = {};
    CGameObject* m_pTarget = nullptr;
    CGameObject* m_pLine = nullptr;
    _float m_fCountDown = 0.f;
    MONSTERSTAT m_tStat;
    SILKWORM_STATE	m_eState;
    _bool m_bPhase2 =false ;
    _bool m_bSpecialAttack = true;
    _bool m_bRotate[3] = {};
    _float			m_fSpeed = 7.f;
    _vec3 m_vRandomPos[8] = {};
    _int m_iHit = 0;
    CGameObject* m_pBeatles[COLOR_END];
    _vec3 m_vOrigin;
    COLOR_STATE m_eCOLORPATTERN=COLOR_BLUE;
    _bool m_bSpawn=false;
    _float m_fiInterval;
    _float m_fEmissive = 1.2f;
    _float m_fAccMotionTrail;
    _float m_fMotionTrailTime;
    
private:
    CUI_BossHP* m_pUIBack;
    CUI_BossHP* m_pUIFrame;
    CUI_BossHP* m_pUIGauge;
};

