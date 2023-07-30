#pragma once
#include "Monster.h"
BEGIN(Engine)
class RcCol;
END
class CSpitCactus :
    public CMonster
{
    CLONE(CSpitCactus)
private:
    explicit CSpitCactus(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSpitCactus(const CSpitCactus& rhs);
    virtual ~CSpitCactus();
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
public:
  static  CSpitCactus* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    // CMonster을(를) 통해 상속됨
    virtual void Trace(_float fTimeDelta) override;
    virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
    void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
    _bool m_bPoison;
    _bool m_bShoot;

};

