#pragma once
#include "GameObject.h"
class CPlayer;

BEGIN(Engine)


class CPushableObj :
    public CGameObject
{

protected:
    CPushableObj(LPDIRECT3DDEVICE9 p_Dev);
    CPushableObj(const CPushableObj& rhs);
    virtual ~CPushableObj();

public:


    virtual void        Push_Obj(const _vec3& pDirection)      PURE;


protected:
    
};

END