#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :	public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix* Get_WorldMatrix() { return &m_matWorld; }
	const _matrix Get_WorldInverseMatrix()
	{
		_matrix	matWorldInv;
		return	*D3DXMatrixInverse(&matWorldInv, nullptr, &m_matWorld);
	}

	void Get_Info(MATRIX_INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

	void Set_Info(MATRIX_INFO eType, _vec3* pInfo)
	{
		memcpy(&m_matWorld.m[eType][0], pInfo, sizeof(_vec3));
	}

	void Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed)
	{
		m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
		Set_Info(INFO_POS, &m_vInfo[INFO_POS]);
	}
	void Move_Pos(OBJ_DIR _eDir, const _float& speed, const _float& fTimeDelta);

	void Set_Pos(const _vec3* const pPos)
	{
		m_vInfo[INFO_POS] = *pPos;
		Set_Info(INFO_POS, &m_vInfo[INFO_POS]);
	}

	void Set_Scale(_vec3 _vScale);
	_vec3 Get_Scale();

	
public:
	void RotationAxis(const _vec3& vAxis, const _float& fAngle);
	const _matrix* Compute_LookAtTarget(const _vec3* pTargetPos);
	


public:
	HRESULT	Ready_Transform();

public:
	_vec3 m_vInfo[INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix	m_matWorld;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void			Free();

};

END