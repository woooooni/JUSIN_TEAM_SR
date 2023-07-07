#include "..\..\Header\Transform.h"

CTransform::CTransform()
	: m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)	
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev, COMPONENT_TYPE::COM_TRANSFORM)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vAngle(rhs.m_vAngle)
	, m_vScale(rhs.m_vScale)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}





const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3 vDir = *pTargetPos - m_vInfo[INFO_POS];

	_matrix	matRot;
	_vec3 vAxis, vUp;

	return D3DXMatrixRotationAxis(&matRot, D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
											acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), 
											D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void CTransform::Move_Pos(OBJ_DIR _eDir, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3 vPos;
	Get_Info(INFO_POS, &vPos);

	_vec3 vDir;
	switch(_eDir)
	{
	case OBJ_DIR::DIR_U:
		vDir = { 0.f, 0.f, 1.f };
		break;
	case OBJ_DIR::DIR_D:
		vDir = { 0.f, 0.f, -1.f };
		break;
	case OBJ_DIR::DIR_L:
		vDir = { -1.f, 0.f, 0.f };
		break;
	case OBJ_DIR::DIR_R:
		vDir = { 1.f, 0.f, 0.f };
		break;
	case OBJ_DIR::DIR_LU:
		vDir = { -1.f, 0.f, 1.f };
		break;
	case OBJ_DIR::DIR_RU:
		vDir = { 1.f, 0.f, 1.f };
		break;
	case OBJ_DIR::DIR_LD:
		vDir = { -1.f, 0.f, -1.f };
		break;
	case OBJ_DIR::DIR_RD:
		vDir = { 1.f, 0.f, -1.f };
		break;
	}

	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * fSpeed * fTimeDelta;

	Set_Info(INFO_POS, &vPos);
}

void CTransform::Set_Scale(_vec3 _vScale)
{
	m_matWorld._11 = _vScale.x;
	m_matWorld._22 = _vScale.y;
	m_matWorld._33 = _vScale.z;
}

_vec3 CTransform::Get_Scale()
{
	_vec3 vRight, vUp, vLook;
	Get_Info(INFO_RIGHT, &vRight);
	Get_Info(INFO_UP, &vUp);
	Get_Info(INFO_LOOK, &vLook);

	return _vec3(D3DXVec3Length(&vRight),
					D3DXVec3Length(&vUp),
					D3DXVec3Length(&vLook));
}

void CTransform::RotationAxis(const _vec3 & vAxis, const _float & fAngle)
{
	_vec3		vRight, vUp, vLook;

	Get_Info(INFO_RIGHT, &vRight);
	Get_Info(INFO_UP, &vUp);
	Get_Info(INFO_LOOK, &vLook);

	_matrix	matRotation;

	D3DXMatrixRotationAxis(&matRotation, &vAxis, fAngle);

	D3DXVec3TransformNormal(&vRight, &vRight, &matRotation);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRotation);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotation);

	Set_Info(INFO_RIGHT, &vRight);
	Set_Info(INFO_UP, &vUp);
	Set_Info(INFO_LOOK, &vLook);
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));


	return S_OK;
}


CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform*		pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}

//
//void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed)
//{
//	_vec3 vDir = *pTargetPos - m_vInfo[INFO_POS];
//	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;
//
//	_matrix	matRot = *Compute_LookAtTarget(pTargetPos);
//	_matrix	matScale, matTrans;
//
//	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
//	D3DXMatrixTranslation(&matTrans,
//		m_vInfo[INFO_POS].x,
//		m_vInfo[INFO_POS].y,
//		m_vInfo[INFO_POS].z);
//
//	m_matWorld = matScale * matRot * matTrans;
//}
