#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemLeftHand : public CGolemPart
{
	CLONE(CGolemLeftHand)

protected:
	explicit CGolemLeftHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemLeftHand(const CGolemLeftHand& rhs);
	virtual ~CGolemLeftHand();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:
	
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemLeftHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void Set_ArmNum(_int _iNum) { m_iArmNum = _iNum; }

private:
	_int m_iArmNum = 0;
	_float m_fAlpha = 255.f;
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};