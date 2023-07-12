#pragma once
#include "Monster.h"

// 패턴 수정 필요 //

BEGIN(Engine)

class RcCol;

END

typedef enum class BugColorType
{
    YELLOW, BLUE, PINK,
    BUGCOLOR_END

}BUGCOLORTYPE;

struct tagRollingBugInfo
{
    BUGCOLORTYPE    eType;
    _vec3           vDefaultPos;
};

class CRollingBug : public CMonster
{
    CLONE(CRollingBug);

private:
    explicit CRollingBug(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRollingBug(const CRollingBug& rhs);
    virtual ~CRollingBug();

public:
    void        Set_Info(const _vec3 vPos, BUGCOLORTYPE eType);

public:
    virtual HRESULT Ready_Object(void)							override;
    virtual _int	Update_Object(const _float& fTimeDelta)     override;
    virtual void	LateUpdate_Object(void)						override;
    virtual void    Render_Object(void)                         override;

public:
    virtual void    Update_Idle(_float fTimeDelta)   override;
    virtual void    Update_Die(_float fTimeDelta)    override;
    virtual void    Update_Regen(_float fTimeDelta)  override;
    virtual void    Update_Move(_float fTimeDelta)   override;
    virtual void    Update_Attack(_float fTimeDelta) override;

private:
    virtual void    Trace(_float fTimeDelta)         override;

private:
    tagRollingBugInfo   m_tBugInfo;
//    _vec3               m_vDefaultPos;
    _vec3               m_vDst = { 0.f, 10.f, 0.f };
    _vec3               m_vDir = { 0.f, 0.f, 0.f };
    _float              m_fMoveTime;

public:
    HRESULT	    Add_Component(void);

public:
    static  CRollingBug* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, BUGCOLORTYPE eType);
};

