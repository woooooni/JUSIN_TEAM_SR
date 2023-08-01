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
	virtual void Update_Idle(_float fTimeDelta)		PURE;
	virtual void Update_Regen(_float fTimeDelta)	PURE;
	virtual void Update_Move(_float fTimeDelta)		PURE;
	virtual void Update_Attack(_float fTimeDelta)	PURE;
	virtual void Update_Die(_float fTimeDelta)		PURE;
public:
	SUNGOLEM_STATE Get_State() { return m_eState; }
	void	Set_State(SUNGOLEM_STATE _eState) { if (m_eState == _eState) return; m_eState = _eState; }
	void		Set_Offset(_vec3 _Offset) { m_vOffset = _Offset; }
	_vec3 		Get_Offset() { return m_vOffset; }
	void		Set_Index(_int _iIndex) { m_iIndex = _iIndex; }
	void		Move_to_Offset(_vec3 _vPos);
	void		Move_Offset(_vec3 _vPos, _float fTimeDelta, _float _fSpeed);
	void		Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	void		Rotate();
	_float		Get_RotationAngle() { return m_fRotationAngle; }
	void		Set_RotationAngle(_float _fRotationAngle) { m_fRotationAngle = _fRotationAngle; }
protected:
	SUNGOLEM_STATE	m_eState;
	CGameObject*	m_pTarget;
	_int m_iIndex;
	_vec3	m_vOffset;
	_float m_fRotationAngle;
	_bool m_bExhale = false;
	_float m_fAccMotionTrail;
	_float m_fMotionTrailTime;
protected:
	void Move_Offset_ByDir(_vec3 _vDir, _float fTimeDelta, _float _fSpeed);
	virtual void Free() override;
};

