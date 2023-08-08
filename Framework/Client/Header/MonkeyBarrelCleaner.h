#pragma once
#include "Monster.h"
BEGIN(Engine)
class RcCol;
END
class CMonkeyBarrelCleaner :
    public CMonster
{
    CLONE(CMonkeyBarrelCleaner)
private:
    explicit CMonkeyBarrelCleaner(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMonkeyBarrelCleaner(const CMonkeyBarrelCleaner& rhs);
    virtual ~CMonkeyBarrelCleaner();
public:
    virtual void Update_Idle(_float fTimeDelta) override;
    virtual void Update_Die(_float fTimeDelta) override;
    virtual void Update_Regen(_float fTimeDelta) override;
    virtual void Update_Move(_float fTimeDelta) override;
    virtual void Update_Attack(_float fTimeDelta) override;
public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)		override;
    virtual void    Render_Object(void)                         override;
    virtual void	LateUpdate_Object(void)						override;

    HRESULT	Add_Component(void);

    void Set_Right(_bool _bRight);
public:
  static  CMonkeyBarrelCleaner* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    // CMonster을(를) 통해 상속됨
    virtual void Trace(_float fTimeDelta) override;
    void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
    _bool m_bRight;
    _bool m_bShoot;
    _float m_fMoveTime;
};

