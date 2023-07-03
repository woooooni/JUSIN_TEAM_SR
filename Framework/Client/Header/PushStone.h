#pragma once
#include "InteractiveObj.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;

END
class CPushStone :
    public CInteractiveObj
{
private:
    CPushStone(LPDIRECT3DDEVICE9 pDev);
    CPushStone(const CPushStone& rhs);
    ~CPushStone();

public:
    virtual HRESULT		Ready_Object(void) override;
    virtual _int		Update_Object(const _float& fTimeDelta) override;
    virtual void		LateUpdate_Object(void) override;
    virtual void		Render_Object(void) override;



    // CInteractiveObj을(를) 통해 상속됨
    virtual HRESULT Grap_Obj(CPlayer* p_Owner) override;
    virtual HRESULT Fall_Obj() override;
    virtual void Push_Obj(const _vec3& pDirection) override;

    virtual void    Free() override;

    static      CPushStone* Create(const _vec3& p_Pos, LPDIRECT3DDEVICE9 pGraphicDev);

private:
    HRESULT                 Ready_Component();

    CRcTex* m_pRcTex = nullptr;
    CCubeTex* m_pCubeTex = nullptr;
};