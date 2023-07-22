#pragma once
#include "FieldObject.h"
class CHitObj : public CFieldObject
{
	CLONE(CHitObj)
protected:
	CHitObj(LPDIRECT3DDEVICE9 p_Dev);
	CHitObj(const CHitObj& rhs);
	virtual ~CHitObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CHitObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos = { 0, 0, 0 }, const _tchar* statuename = L"Monkey");

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;

public:
	void			Set_Event(const _uint& pI) { m_iEventNum = pI; }
	void			Set_HitType(const OBJ_HITTYPE& pH) { m_eHitType = pH; }
protected:
	CAnimator* m_pBlurAnimator;
	OBJ_HITTYPE			m_eHitType;
	HRESULT				Ready_Component();
	float				m_fEffectTime;
	float				m_fResetTime;
	_uint				m_iEventNum;
	_bool				m_bHitted = false;
	_vec3				m_vOrigin;
	_vec3				m_vToward;

	void				Make_Toward();
	

};

