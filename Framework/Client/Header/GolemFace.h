#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CGolemFace : public Engine::CGameObject
{
protected:
	explicit CGolemFace(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemFace(const CGolemFace& rhs);
	virtual ~CGolemFace();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:

	SUNGOLEM_STATE Get_State() { return m_eState; }
	void Set_State(SUNGOLEM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }

public:
	void Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
public:
	virtual void Update_Idle(_float fTimeDelta)	;
	virtual void Update_Dirty(_float fTimeDelta);
	virtual void Update_Move(_float fTimeDelta)	;
	virtual void Update_Attack(_float fTimeDelta);
	virtual void Update_Die(_float fTimeDelta)	;
	virtual void Update_Regen(_float fTimeDelta);

	static CGolemFace* Create(LPDIRECT3DDEVICE9 pGraphicDev);



private:
	SUNGOLEM_STATE	m_eState;
protected:
	CGameObject* m_pTarget;
	_float m_fMoveTime;
	bool m_bBreath=false;
protected:
	virtual void Free() override;

};