#include "..\..\Header\Collider.h"
#include "GameObject.h"
#include "Transform.h"


UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_bEnable(true)
	, m_bRender(true)
	, m_iID(g_iNextID++)
{
}

CCollider::CCollider(LPDIRECT3DDEVICE9 _pDevice, COMPONENT_TYPE _eComType, COLLIDER_TYPE _eColType)
	: CComponent(_pDevice, _eComType)
	, m_bEnable(true)
	, m_bRender(true)
	, m_iID(g_iNextID++)
	, m_eType(_eColType)
	, m_vOffset(_vec3(0.f, 0.f, 0.f))
{
	ZeroMemory(m_vCenterPos, sizeof(D3DXVECTOR3));
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_bEnable(rhs.m_bEnable)
	, m_bRender(rhs.m_bRender)
	, m_iID(g_iNextID++)
	, m_eType(rhs.m_eType)
	, m_vOffset(rhs.m_vOffset)
{
	CopyMemory(m_vCenterPos, rhs.m_vCenterPos, sizeof(D3DXVECTOR3));
}


CCollider::~CCollider()
{
}
//
//void CheckOBB(CCollider* targetBox)
//
//{
//
//	double c[3][3];
//
//	double absC[3][3];
//
//	double d[3];
//
//	double r0, r1, r;
//
//	int i;
//
//	const double cutoff = 0.999999;
//
//	bool existsParallelPair = false;
//
//	D3DXVECTOR3 diff = boundingBox.centerPos - targetBox.centerPos;
//
//
//
//	for (i = 0; i < 3; ++i)
//
//	{
//
//		c[0][i] = D3DXVec3Dot(&boundingBox.axis[0], &targetBox.axis[i]);
//
//		absC[0][i] = abs(c[0][i]);
//
//		if (absC[0][i] > cutoff)
//
//			existsParallelPair = true;
//
//	}
//
//
//
//	d[0] = D3DXVec3Dot(&diff, &boundingBox.axis[0]);
//
//	r = abs(d[0]);
//
//	r0 = boundingBox.axisLen[0];
//
//	r1 = targetBox.axisLen[0] * absC[0][0] + targetBox.axisLen[1] * absC[0][1] + targetBox.axisLen[2] * absC[0][2];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//	for (i = 0; i < 3; ++i)
//
//	{
//
//		c[1][i] = D3DXVec3Dot(&boundingBox.axis[1], &targetBox.axis[i]);
//
//		absC[1][i] = abs(c[1][i]);
//
//		if (absC[1][i] > cutoff)
//
//			existsParallelPair = true;
//
//	}
//
//	d[1] = D3DXVec3Dot(&diff, &boundingBox.axis[1]);
//
//	r = abs(d[1]);
//
//	r0 = boundingBox.axisLen[1];
//
//	r1 = targetBox.axisLen[0] * absC[1][0] + targetBox.axisLen[1] * absC[1][1] + targetBox.axisLen[2] * absC[1][2];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//	for (i = 0; i < 3; ++i)
//
//	{
//
//		c[2][i] = D3DXVec3Dot(&boundingBox.axis[2], &targetBox.axis[i]);
//
//		absC[2][i] = abs(c[2][i]);
//
//		if (absC[2][i] > cutoff)
//
//			existsParallelPair = true;
//
//	}
//
//	d[2] = D3DXVec3Dot(&diff, &boundingBox.axis[2]);
//
//	r = abs(d[2]);
//
//	r0 = boundingBox.axisLen[2];
//
//	r1 = targetBox.axisLen[0] * absC[2][0] + targetBox.axisLen[1] * absC[2][1] + targetBox.axisLen[2] * absC[2][2];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//	r = abs(D3DXVec3Dot(&diff, &targetBox.axis[0]));
//
//	r0 = boundingBox.axisLen[0] * absC[0][0] + boundingBox.axisLen[1] * absC[1][0] + boundingBox.axisLen[2] * absC[2][0];
//
//	r1 = targetBox.axisLen[0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(D3DXVec3Dot(&diff, &targetBox.axis[1]));
//
//	r0 = boundingBox.axisLen[0] * absC[0][1] + boundingBox.axisLen[1] * absC[1][1] + boundingBox.axisLen[2] * absC[2][1];
//
//	r1 = targetBox.axisLen[1];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(D3DXVec3Dot(&diff, &targetBox.axis[2]));
//
//	r0 = boundingBox.axisLen[0] * absC[0][2] + boundingBox.axisLen[1] * absC[1][2] + boundingBox.axisLen[2] * absC[2][2];
//
//	r1 = targetBox.axisLen[2];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	if (existsParallelPair == true)
//
//	{
//
//		obbCollide = true;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
//
//	r0 = boundingBox.axisLen[1] * absC[2][0] + boundingBox.axisLen[2] * absC[1][0];
//
//	r1 = targetBox.axisLen[1] * absC[0][2] + targetBox.axisLen[2] * absC[0][1];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
//
//	r0 = boundingBox.axisLen[1] * absC[2][1] + boundingBox.axisLen[2] * absC[1][1];
//
//	r1 = targetBox.axisLen[0] * absC[0][2] + targetBox.axisLen[2] * absC[0][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
//
//	r0 = boundingBox.axisLen[1] * absC[2][2] + boundingBox.axisLen[2] * absC[1][2];
//
//	r1 = targetBox.axisLen[0] * absC[0][1] + targetBox.axisLen[1] * absC[0][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
//
//	r0 = boundingBox.axisLen[0] * absC[2][0] + boundingBox.axisLen[2] * absC[0][0];
//
//	r1 = targetBox.axisLen[1] * absC[1][2] + targetBox.axisLen[2] * absC[1][1];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
//
//	r0 = boundingBox.axisLen[0] * absC[2][1] + boundingBox.axisLen[2] * absC[0][1];
//
//	r1 = targetBox.axisLen[0] * absC[1][2] + targetBox.axisLen[2] * absC[1][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
//
//	r0 = boundingBox.axisLen[0] * absC[2][2] + boundingBox.axisLen[2] * absC[0][2];
//
//	r1 = targetBox.axisLen[0] * absC[1][1] + targetBox.axisLen[1] * absC[1][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
//
//	r0 = boundingBox.axisLen[0] * absC[1][0] + boundingBox.axisLen[1] * absC[0][0];
//
//	r1 = targetBox.axisLen[1] * absC[2][2] + targetBox.axisLen[2] * absC[2][1];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
//
//	r0 = boundingBox.axisLen[0] * absC[1][1] + boundingBox.axisLen[1] * absC[0][1];
//
//	r1 = targetBox.axisLen[0] * absC[2][2] + targetBox.axisLen[2] * absC[2][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
//
//	r0 = boundingBox.axisLen[0] * absC[1][2] + boundingBox.axisLen[1] * absC[0][2];
//
//	r1 = targetBox.axisLen[0] * absC[2][1] + targetBox.axisLen[1] * absC[2][0];
//
//	if (r > r0 + r1)
//
//	{
//
//		obbCollide = false;
//
//		return;
//
//	}
//
//
//
//	obbCollide = true;
//
//	return;
//
//}

