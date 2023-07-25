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

    // BossHPBar 연동으로 인해 추가함 (Test)
    MONSTERSTAT Get_Stat() { return m_tStat; }
public:
    static  CSilkWorm* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    SILKWORM_STATE Get_State() { return m_eState; }
    void Set_State(SILKWORM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }

private:
    _float m_fMoveTime;
    _float m_fEffectCoolTime =0.f;
    _vec3 m_vDst = { 0.f,0.f,0.f };
    _vec3 m_vDir = { 0.f,0.f,0.f };
    _bool m_bShoot = true;
     void Trace(_float fTimeDelta);
     void Create_Effect(_vec3 vPos);
     void Create_Line();
     void Create_NueFlower();
    void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
    CGameObject* m_pNueFlower[6] = {};
    CGameObject* m_pTarget = nullptr;
    CGameObject* m_pLine = nullptr;
    MONSTERSTAT m_tStat;
    SILKWORM_STATE	m_eState;
    _bool m_bPhase2 =false ;
    _bool m_bRotate[3] = {};
    _float			m_fSpeed = 7.f;
    _vec3 m_vRandomPos[8] = {};
    _int m_iHit = 0;
    CGameObject* m_pBeatles[COLOR_END];
    _vec3 m_vOrigin;
    COLOR_STATE m_eCOLORPATTERN=COLOR_END;
    _bool m_bSpawn=false;
private:
    CUI_BossHP* m_pUIBack;
    CUI_BossHP* m_pUIFrame;
    CUI_BossHP* m_pUIGauge;
};

