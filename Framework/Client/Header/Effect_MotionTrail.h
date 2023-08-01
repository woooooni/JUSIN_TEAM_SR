#pragma once
#include "Effect.h"
class CEffect_MotionTrail :
    public CEffect
{
    CLONE(CEffect_MotionTrail)
private:
    explicit CEffect_MotionTrail(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CEffect_MotionTrail();


public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)		override;
    virtual void	LateUpdate_Object(void)						override;
    virtual void	Render_Object(void)							override;

public:
    virtual void Return_Pool(void) override;

public:
    void Set_Texture(LPDIRECT3DBASETEXTURE9 pTex) { m_pTex = pTex; }

private:
    HRESULT Ready_Component(void);

private:
    _float m_fAccDelete;
    _float m_fDeleteTime;
    CGameObject* m_pOwner;
    LPDIRECT3DBASETEXTURE9 m_pTex;
    _float m_fAlpha = 255.f;
public:
    static CEffect_MotionTrail* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
    virtual void Free() override;
};

