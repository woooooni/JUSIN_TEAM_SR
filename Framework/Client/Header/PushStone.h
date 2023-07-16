#pragma once
#include "FieldObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;

END

class CPushStone : public CFieldObject
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



    static      CPushStone* Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev);

    virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
    virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;

    void            Fire() { m_bIsFlying = true; };

    _bool Is_Flying() { return m_bIsFlying; }

    void        Set_Clean() { m_bIsClean = true; }
    const _bool& Get_Clean() { return m_bIsClean; }
    void        Reset() { m_bIsClean = false; m_bIsFlying = false; }

private:
    HRESULT                 Ready_Component();

    _bool           m_bIsFlying;
    _bool           m_bIsClean;

    CRcTex* m_pRcTex = nullptr;
    CCubeTex* m_pCubeTex = nullptr;
};