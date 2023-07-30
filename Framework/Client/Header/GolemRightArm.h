#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemRightArm : public CGolemPart
{
	CLONE(CGolemRightArm)
protected:
	explicit CGolemRightArm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemRightArm(const CGolemRightArm& rhs);
	virtual ~CGolemRightArm();

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

	static CGolemRightArm* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void Set_ArmNum(_int _iNum) { m_iArmNum = _iNum; }
private:
	_float m_fAlpha = 255.f;
	_int m_iArmNum = 0;
protected:
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};