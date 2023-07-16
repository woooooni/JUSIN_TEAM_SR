#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemLowerJaw : public CGolemPart
{
	CLONE(CGolemLowerJaw)
protected:
	explicit CGolemLowerJaw(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemLowerJaw(const CGolemLowerJaw& rhs);
	virtual ~CGolemLowerJaw();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemLowerJaw* Create(LPDIRECT3DDEVICE9 pGraphicDev);



private:

	bool m_bBreath = false;
protected:
	CGameObject* m_pTarget;
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};