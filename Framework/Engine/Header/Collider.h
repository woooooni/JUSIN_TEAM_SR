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

	UINT			Get_Id() { return m_iID; }

public:
	virtual void	OnCollisionEnter(CCollider* _pOther)		PURE;
	virtual void	OnCollisionStay(CCollider* _pOther)		PURE;
	virtual void	OnCollisionExit(CCollider* _pOther)		PURE;

protected:
	D3DXVECTOR3		m_vCenterPos;	// ���� �߾��� ��ǥ
	D3DXVECTOR3		m_vAxisDir[3];	//���ڿ� ������ �� ���� ��������
	float			m_fAxisLen[3];
	// ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.

protected:
	_bool			m_bRender;
	_bool			m_bEnable;

private:
	static UINT		g_iNextID;
	
	UINT			m_iID;
	COLLIDER_TYPE	m_eType;
};

END
