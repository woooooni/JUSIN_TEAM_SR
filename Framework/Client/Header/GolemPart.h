#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CGolemPart : public Engine::CGameObject
{
protected:
	explicit CGolemPart(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGolemPart(const CGolemPart& rhs);
	virtual ~CGolemPart();

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

	void	Set_Offset(_vec3 _Offset) { m_vOffset = _Offset; }
	void   Set_Index(_int _iIndex) { m_iIndex = _iIndex; }
	void	Offset(_vec3 _vPos);
	void Move_Offset(_vec3 _vPos, _float fTimeDelta, _float _fSpeed);

public:
	void		 Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	
public:
	virtual void Update_Idle(_float fTimeDelta)		PURE;
	virtual void Update_Regen(_float fTimeDelta)	PURE;
	virtual void Update_Move(_float fTimeDelta)		PURE;
	virtual void Update_Attack(_float fTimeDelta)	PURE;
	virtual void Update_Die(_float fTimeDelta)		PURE;
	
protected:
	SUNGOLEM_STATE	m_eState;

protected:
	CGameObject*	m_pTarget;
	_int m_iIndex;
	_vec3	m_vOffset;
protected:
	void Move_Offset_ByDir(_vec3 _vDir, _float fTimeDelta, _float _fSpeed);
	virtual void Free() override;
};

