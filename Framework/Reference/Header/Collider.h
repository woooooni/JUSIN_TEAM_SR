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

	void			SetAxisLen(MATRIX_INFO eInfo, _float _f) { m_fAxisLen[eInfo] = _f; }

public:
	virtual void	OnCollisionEnter(CCollider* _pOther)	PURE;
	virtual void	OnCollisionStay(CCollider* _pOther)		PURE;
	virtual void	OnCollisionExit(CCollider* _pOther)		PURE;

protected:
	D3DXVECTOR3		m_vCenterPos;	// 상자 중앙의 좌표
	D3DXVECTOR3		m_vAxisDir[3];	//상자에 평행한 세 축의 단위벡터
	float			m_fAxisLen[3];
	// 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.

private:
	static UINT		g_iNextID;
	bool			m_bEnable;
	bool			m_bRender;

	UINT			m_iID;
	COLLIDER_TYPE	m_eType;
};

END
