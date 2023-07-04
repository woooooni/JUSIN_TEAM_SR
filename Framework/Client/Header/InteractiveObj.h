#pragma once
#include "GameObject.h"
class CPlayer;

BEGIN(Engine)


class CInteractiveObj :
    public CGameObject
{

protected:
    CInteractiveObj(LPDIRECT3DDEVICE9 p_Dev);
    CInteractiveObj(const CInteractiveObj& rhs);
    ~CInteractiveObj();

public:

    virtual HRESULT     Grap_Obj(CPlayer* p_Owner)    PURE;
    virtual HRESULT     Fall_Obj()                    PURE;

    virtual void        Push_Obj(const _vec3& pDirection)      PURE;


protected:
    
};

END