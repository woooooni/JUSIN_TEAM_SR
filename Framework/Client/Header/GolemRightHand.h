#pragma once
#include "GolemPart.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemRightHand : public CGolemPart
{
	CLONE(CGolemRightHand)
protected:
	explicit CGolemRightHand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemRightHand(const CGolemRightHand& rhs);
	virtual ~CGolemRightHand();

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

	static CGolemRightHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void Set_ArmNum(_int _iNum) { m_iArmNum = _iNum; }
private:
	_int m_iArmNum = 0;
protected:
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};