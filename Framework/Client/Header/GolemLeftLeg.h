#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemLeftLeg : public CGolemPart
{
	CLONE(CGolemLeftLeg)
protected:
	explicit CGolemLeftLeg(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemLeftLeg(const CGolemLeftLeg& rhs);
	virtual ~CGolemLeftLeg();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);



	
public:
	void Set_Target(CGolemPart* _pTarget) { m_pTarget = _pTarget; }
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemLeftLeg* Create(LPDIRECT3DDEVICE9 pGraphicDev);



private:

	bool m_bBreath = false;

	_float m_fMoveTime;
protected:
	virtual void Free() override;

};