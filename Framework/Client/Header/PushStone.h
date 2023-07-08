#pragma once
#include "PushableObj.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;

END
class CPushStone :
    public CPushableObj
{
    CLONE(CPushStone)
private:
    CPushStone(LPDIRECT3DDEVICE9 pDev);
    CPushStone(const CPushStone& rhs);
    virtual ~CPushStone();

public:
    virtual HRESULT		Ready_Object(void) override;
    virtual _int		Update_Object(const _float& fTimeDelta) override;
    virtual void		LateUpdate_Object(void) override;
    virtual void		Render_Object(void) override;

    virtual void    Free() override;


    // CInteractiveObj을(를) 통해 상속됨
    virtual void Push_Obj(const _vec3& pDirection) override;


    static      CPushStone* Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev);

    virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)override;

    void            Fire() { m_bIsFlying = true; };

private:
    HRESULT                 Ready_Component();

    _bool           m_bIsFlying;

    CRcTex* m_pRcTex = nullptr;
    CCubeTex* m_pCubeTex = nullptr;
};