#pragma once
#include "FieldObject.h"
class CPlayer;

BEGIN(Engine)


class CPushableObj :
    public CFieldObject
{

protected:
    CPushableObj(LPDIRECT3DDEVICE9 p_Dev, OBJ_ID _eID);
    CPushableObj(const CPushableObj& rhs);
    virtual ~CPushableObj();

public:
    virtual void        Push_Obj(const _vec3& pDirection)      PURE;

protected:
    
};

END