#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider :
	public CComponent
{
public:
	explicit CCollider();
	explicit CCollider(LPDIRECT3DDEVICE9 _pDevice, COMPONENT_TYPE _eComType, COLLIDER_TYPE _eColType);
	explicit CCollider(const CCollider & rhs);
	virtual ~CCollider();

public:
	bool			IsEnable()			{ return m_bEnable; }
	void			SetEnable(bool _b)	{ m_bEnable = _b; }

	COLLIDER_TYPE	GetColliderType()	{ return m_eType; }
	UINT			Get_Id() { return m_iID; }

public:
	void Set_Offset(_vec3 vOffset) { m_vOffset = vOffset; }
	const _vec3& Get_Offset() { return m_vOffset; }

public:
	virtual void	OnCollisionEnter(CCollider* _pOther, COLLISION_GROUP _eGroup)		PURE;
	virtual void	OnCollisionStay(CCollider* _pOther, COLLISION_GROUP _eGroup)			PURE;
	virtual void	OnCollisionExit(CCollider* _pOther, COLLISION_GROUP _eGroup)			PURE;




protected:
	D3DXVECTOR3		m_vCenterPos;	// »óÀÚ Áß¾ÓÀÇ ÁÂÇ¥
	_vec3			m_vOffset;

protected:
	_bool			m_bRender;
	_bool			m_bEnable;

private:
	static UINT		g_iNextID;
	
	UINT			m_iID;
	COLLIDER_TYPE	m_eType;
};

END
