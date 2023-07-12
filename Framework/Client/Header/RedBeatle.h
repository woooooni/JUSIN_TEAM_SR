#pragma once
#include "Monster.h"

BEGIN(Engine)
class RcCol;
END

class CRedBeatle :
    public CMonster
{
    CLONE(CRedBeatle)

private:
    explicit CRedBeatle(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRedBeatle(const CRedBeatle& rhs);
    virtual ~CRedBeatle();

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

private:
    HRESULT	     Add_Component(void);
    void         Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);
    virtual void Trace(_float fTimeDelta) override;

private:
    _float          m_fMoveTime;
    _vec3           m_vDst = { 0,1,0 };

public:
    static  CRedBeatle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

