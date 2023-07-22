#pragma once
#include "Monster.h"

BEGIN(Engine)
class RcCol;
END

class CCupa : public CMonster
{
    CLONE(CCupa);

private:
    explicit CCupa(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCupa(const CCupa& rhs);
    virtual ~CCupa();

public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)     override;
    virtual void	LateUpdate_Object(void)						override;
    virtual void    Render_Object(void)                         override;

public:
    virtual void    Update_Idle(_float fTimeDelta)   override;
    virtual void    Update_Die(_float fTimeDelta)    override;
    virtual void    Update_Regen(_float fTimeDelta)  override;
    virtual void    Update_Move(_float fTimeDelta)   override;
    virtual void    Update_Attack(_float fTimeDelta) override;

private:
    virtual void    Trace(_float fTimeDelta)         override;
    virtual void    Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
    void            Set_Animation();
private:
    _float      m_fMoveTime;
    _vec3       m_vDst = { 0.f, 10.f, 0.f };
    _vec3       m_vDir = { 0.f, 0.f, 0.f };
    _vec3       m_vArrived = { 0.f, 0.f, 0.f };
    _bool       m_bJump = false;

public:
    HRESULT	    Add_Component(void);

public:
    static  CCupa* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

