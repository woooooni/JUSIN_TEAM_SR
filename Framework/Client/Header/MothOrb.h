#pragma once
#include "Item.h"
BEGIN(Engine)
class RcCol;
END
class CMothOrb :
    public CItem
{
    CLONE(CMothOrb)
private:
    explicit CMothOrb(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMothOrb(const CMothOrb& rhs);
    virtual ~CMothOrb();
public:

public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)		override;
    virtual void Render_Object(void) override;
    virtual void	LateUpdate_Object(void)						override;

    HRESULT	Add_Component(void);
public:
    static  CMothOrb* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _vec3 m_vDir;
};

