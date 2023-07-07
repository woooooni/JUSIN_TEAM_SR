#include "Export_Function.h"
#include "RigidBody.h"
#include <iostream>


CRigidBody::CRigidBody()
	: m_fMass(1.f)
	, m_vMaxVelocity(_vec3(1000.f, 1500.f, 1000.f))
	, m_bGravity(true)
	, m_bGround(false)
	, m_vVelocity(_vec3(0.f, 0.f, 0.f))
	, m_vAccel(_vec3(0.f, 0.f, 0.f))
	, m_vAccelA(_vec3(0.f, 0.f, 0.f))
	, m_vForce(_vec3(0.f, 0.f, 0.f))
	, m_fFricCoeff(100.f)
{
}

CRigidBody::CRigidBody(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice, COMPONENT_TYPE::COM_RIGIDBODY)
	, m_fMass(10.f)
	, m_vMaxVelocity(_vec3(1000.f, 1500.f, 1000.f))
	, m_bGravity(true)
	, m_bGround(false)
	, m_vVelocity(_vec3(0.f, 0.f, 0.f))
	, m_vAccel(_vec3(0.f, 0.f, 0.f))
	, m_vAccelA(_vec3(0.f, 0.f, 0.f)) 
	, m_vForce(_vec3(0.f, 0.f, 0.f))
	, m_fFricCoeff(100.f)
{
}

CRigidBody::CRigidBody(const CRigidBody& rhs)
	: CComponent(rhs)
	, m_fMass(rhs.m_fMass)
	, m_vMaxVelocity(rhs.m_vMaxVelocity)
	, m_bGravity(true)
	, m_bGround(false)
	, m_vVelocity(_vec3(0.f, 0.f, 0.f))
	, m_vAccel(_vec3(0.f, 0.f, 0.f))
	, m_vAccelA(_vec3(0.f, 0.f, 0.f))
	, m_vForce(_vec3(0.f, 0.f, 0.f))
	, m_fFricCoeff(rhs.m_fFricCoeff)
{
}

CRigidBody::~CRigidBody()
{
}

HRESULT CRigidBody::Ready_RigidBody()
{
    return S_OK;
}

_int CRigidBody::Update_Component(const _float& fTimeDelta)
{
	CTransform* pOwnerTransform = m_pOwner->Get_TransformCom();
	_vec3 vPos;
	pOwnerTransform->Get_Info(INFO_POS, &vPos);

	if (vPos.y < 1.f)
	{
		vPos.y = 1.f;
		pOwnerTransform->Set_Info(INFO_POS, &vPos);
		SetGround(true);
	}

	if (IsGravity() && IsGround() == false)
		Update_Gravity();


	_float fForce = D3DXVec3Length(&m_vForce);
	if (0.f != fForce)
	{
		_vec3 vNormalForce;
		D3DXVec3Normalize(&vNormalForce, &m_vForce);

		_float fAccel = fForce / m_fMass;

		m_vAccel = m_vForce * fAccel;
	}

	m_vAccel += m_vAccelA;
	m_vVelocity += m_vAccel * fTimeDelta;

	if (m_vVelocity.x > 0.f || m_vVelocity.z > 0.f)
	{
		_vec3 vFricDir = m_vVelocity * -1.f;
		D3DXVec3Normalize(&vFricDir, &vFricDir);

		_vec3 vFriction;

		if (IsGround())
			vFriction = vFricDir * m_fFricCoeff * fTimeDelta;
		else
			vFriction = vFricDir * fTimeDelta;
		if (D3DXVec3Length(&m_vVelocity) <= D3DXVec3Length(&vFriction))
		{
			m_vVelocity = _vec3(0.f, 0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);

	if (abs(m_vMaxVelocity.z) < abs(m_vVelocity.z))
		m_vVelocity.z = (m_vVelocity.z / abs(m_vVelocity.z)) * abs(m_vMaxVelocity.z);

	
	std::cout << "Velocity : " << m_vVelocity.x << "," << m_vVelocity.y << "," << m_vVelocity.z << endl;
	std::cout << "Accel : " << m_vAccel.x << "," << m_vAccel.y << "," << m_vAccel.z << endl;
	Move(fTimeDelta);


	m_vForce = _vec3(0.f, 0.f, 0.f);
	m_vAccelA = _vec3(0.f, 0.f, 0.f);
	m_vAccel = _vec3(0.f, 0.f, 0.f);


    return S_OK;
}

void CRigidBody::LateUpdate_Component()
{
}

void CRigidBody::Render_Component()
{
}

void CRigidBody::SetGround(_bool _bGround)
{
	m_bGround = _bGround;

	if (IsGround())
		SetVelocity(_vec3(m_vVelocity.x, 0.f, m_vVelocity.z));
}

void CRigidBody::Update_Gravity()
{
	SetAccelAlpha(_vec3(0.f, -98.f, 0.f));
}

void CRigidBody::Move(_float fTimeDelta)
{

	_float fSpeed = D3DXVec3Length(&m_vVelocity);

	if (0.f != fSpeed)
	{
		_vec3 vDir = m_vVelocity;
		D3DXVec3Normalize(&vDir, &m_vVelocity);

		CTransform* pOwnerTransform = m_pOwner->Get_TransformCom();

		_vec3 vPos;

		pOwnerTransform->Get_Info(INFO_POS, &vPos);
		vPos += m_vVelocity * fTimeDelta;
		pOwnerTransform->Set_Info(INFO_POS, &vPos);
	}

}

CRigidBody* CRigidBody::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CRigidBody* pInstance = new CRigidBody(_pDevice);

	if (FAILED(pInstance->Ready_RigidBody()))
	{
		MSG_BOX("RigidBody Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRigidBody::Free()
{
	__super::Free();
}

CComponent* CRigidBody::Clone(void)
{
	return new CRigidBody(*this);
}
