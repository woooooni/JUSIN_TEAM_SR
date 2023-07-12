#pragma once
#include "Monster.h"

BEGIN(Engine)
class RcCol;
END

typedef enum class BeatleColorType
{
    REDBEATLE, GREENBEATLE, BLUEBEATLE,
    BEATLECOLOR_END

}BEATLETYPE;

struct tagBeatleInfo
{
    BEATLETYPE  eType;
};

class CBlueBeatle :
    public CMonster
{
    CLONE(CBlueBeatle)

private:
    explicit CBlueBeatle(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBlueBeatle(const CBlueBeatle& rhs);
    virtual ~CBlueBeatle();

public:
    virtual void Update_Idle(_float fTimeDelta) override;
    virtual void Update_Die(_float fTimeDelta) override;
    virtual void Update_Regen(_float fTimeDelta) override;
    virtual void Update_Move(_float fTimeDelta) override;
    virtual void Update_Attack(_float fTimeDelta) override;

public:
    virtual HRESULT Ready_Object(void)							override;
    virtual void    Render_Object(void)                         override;
    virtual void	LateUpdate_Object(void)						override;

    HRESULT	Add_Component(void);
    void    Set_Type(BEATLETYPE eType);

public:
  static  CBlueBeatle* Create(LPDIRECT3DDEVICE9 pGraphicDev, BEATLETYPE eType = BEATLETYPE::BLUEBEATLE);

private:
    tagBeatleInfo   m_tInfo = {};
    _float          m_fMoveTime;
    _vec3           m_vDst = { 0,1,0 };
    // CMonster을(를) 통해 상속됨
    virtual void Trace(_float fTimeDelta) override;

};

