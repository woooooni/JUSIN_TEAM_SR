#pragma once
#include "Monster.h"
BEGIN(Engine)
class RcCol;
END
class CDesertRhino :
    public CMonster
{
    CLONE(CDesertRhino)
private:
    explicit CDesertRhino(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDesertRhino(const CDesertRhino& rhs);
    virtual ~CDesertRhino();
public:
    virtual void Update_Idle(_float fTimeDelta) override;
    virtual void Update_Die(_float fTimeDelta) override;
    virtual void Update_Regen(_float fTimeDelta) override;
    virtual void Update_Move(_float fTimeDelta) override;
    virtual void Update_Attack(_float fTimeDelta) override;
public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)		override;
    virtual void Render_Object(void) override;
    virtual void	LateUpdate_Object(void)						override;

    HRESULT	Add_Component(void);
public:
    static  CDesertRhino* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    _float m_fMoveTime;
    _vec3 m_vDst = { 0.f,0.f,0.f };
    _vec3 m_vDir = { 0.f,0.f,0.f };

    virtual void Trace(_float fTimeDelta) override;


};

