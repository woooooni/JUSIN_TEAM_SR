#pragma once
#include "Monster.h"
BEGIN(Engine)
class RcCol;
class CCollider;
END

class CTrashBig :
    public CMonster
{
    CLONE(CTrashBig)
    private:
        explicit CTrashBig(LPDIRECT3DDEVICE9 pGraphicDev);
        explicit CTrashBig(const CTrashBig& rhs);
        virtual ~CTrashBig();
    public:
        virtual void Update_Idle(_float fTimeDelta) override;
        virtual void Update_Die(_float fTimeDelta) override;
        virtual void Update_Regen(_float fTimeDelta) override;
        virtual void Update_Move(_float fTimeDelta) override;
        virtual void Update_Attack(_float fTimeDelta) override;
    public:
        virtual HRESULT Ready_Object(void)							override;
        virtual _int Update_Object(const _float& fTimeDelta)		override;
        virtual void Render_Object(void) override;
        virtual void LateUpdate_Object(void)						override;

        HRESULT	Add_Component(void);
    public:
        static  CTrashBig* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    private:
        _float m_fMoveTime;
        _vec3 m_vDst = { 0.f,0.f,0.f };
        virtual void Trace(_float fTimeDelta) override;
        _bool m_bJump = false;
        void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);


};

