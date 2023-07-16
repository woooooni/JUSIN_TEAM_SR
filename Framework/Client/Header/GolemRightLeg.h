#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;
END

class CGolemRightLeg : public CGolemPart
{
	CLONE(CGolemRightLeg)

protected:
	explicit CGolemRightLeg(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemRightLeg(const CGolemRightLeg& rhs);
	virtual ~CGolemRightLeg();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemRightLeg* Create(LPDIRECT3DDEVICE9 pGraphicDev);


protected:
	virtual void Free() override;

};