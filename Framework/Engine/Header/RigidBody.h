#pragma once
#include "Component.h"


BEGIN(Engine)
class CTexture;
class ENGINE_DLL CRigidBody : public CComponent
{
public:
	explicit CRigidBody();
	explicit CRigidBody(LPDIRECT3DDEVICE9 _pDevice);
	explicit CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody();

public:
	HRESULT			Ready_RigidBody();
	virtual	_int	Update_Component(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Component() override;
	virtual void	Render_Component()	override;

public:
	void AddForce(_vec3 _vForce) { m_vForce += _vForce; }

	void SetMass(_float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }

	void AddVelocity(_vec3 _velocity) { m_vVelocity += _velocity; }
	void SetVelocity(_vec3 _velocity) { m_vVelocity = _velocity; }
	_vec3 GetVelocity() { return m_vVelocity; }
	void SetMaxVelocity(_float _fVelocity) { m_fMaxVelocity = _fVelocity; }

	void SetAccelAlpha(_vec3 _vAccel) { m_vAccelA = _vAccel; }
	_float GetSpeed() { return D3DXVec3Length(&m_vVelocity); }


	void SetGravity(_bool _bGravity) { m_bGravity = _bGravity; }
	bool IsGravity() { return m_bGravity; }

	void SetGround(_bool _bGround);
	bool IsGround() { return m_bGround; }

	void SetFricCoeff(_float _fFric) { m_fFricCoeff = _fFric; }
	_float GetFricCoeff() { return m_fFricCoeff; }

private:
	void Update_Gravity();
	void Move(_float fTimeDelta);

private:
	_vec3		m_vForce;		// 힘
	_vec3		m_vAccel;		// 가속도
	_vec3		m_vAccelA;		// 가속도
	_vec3		m_vVelocity;	// 속도
	_float		m_fMaxVelocity;	// 최대 속도

	_float		m_fMass;		// 무게

	_bool		m_bGravity;		// 중력 적용 여부
	_bool		m_bGround;		// 현재 땅바닥에 있는지.

	_float		m_fFricCoeff;	// 마찰력
	

public:
	static CRigidBody* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void Free();
	virtual CComponent* Clone(void) override;
};

END


