#pragma once
#include "Monster.h"

BEGIN(Engine)

class RcCol;
class CCollider;

END

class CTrashFast : public CMonster
{
    CLONE(CTrashFast)

private:
    explicit CTrashFast(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTrashFast(const CTrashFast& rhs);
    virtual ~CTrashFast();

public:
    virtual void Update_Idle(_float fTimeDelta)     override;
    virtual void Update_Move(_float fTimeDelta)     override;
    virtual void Update_Attack(_float fTimeDelta)   override;
    virtual void Update_Die(_float fTimeDelta)      override;
    virtual void Update_Regen(_float fTimeDelta)    override;

public:
    virtual HRESULT Ready_Object(void)						override;
    virtual _int Update_Object(const _float& fTimeDelta)	override;
    virtual void LateUpdate_Object(void)				    override;
    virtual void Render_Object(void)                        override;

public:
    HRESULT	Add_Component(void);
    void    Set_Animation();

private:
    virtual void Trace(_float fTimeDelta)   override;
    virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

private:
    _float  m_fMoveTime;
    _vec3   m_vDst = { 0.f,0.f,0.f };
    _bool   m_bJump = FALSE;

public:
    static  CTrashFast* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

