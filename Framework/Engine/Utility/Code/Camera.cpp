#include "Camera.h"
#include "Export_Function.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_CAMERA)
	, m_fFov(4.f)
	, m_pTargetObj(nullptr)
	, m_fDist(10.f)
	, m_fFollowSpeed(5.f)
{

}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_fFov(4.f)
	, m_fDist(rhs.m_fDist)
	, m_pTargetObj(rhs.m_pTargetObj)
	, m_fFollowSpeed(rhs.m_fFollowSpeed)
{

}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3( 0.f, 10.f
		, -10.f ));
	return S_OK;
}

_int CCamera::Update_Object(const _float& fTimeDelta)
{
	// Key_Input(fTimeDelta);
	Follow(fTimeDelta);

	_vec3 vPos, vLook, vRight, vUp;
	ZeroMemory(&vPos, sizeof(_vec3));
	ZeroMemory(&vLook, sizeof(_vec3));
	ZeroMemory(&vUp, sizeof(_vec3));

	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_LOOK, &vLook);

	D3DXMatrixLookAtLH(&m_matView, &vPos, &vLook, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / m_fFov, WINCX / WINCY, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CCamera::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCamera::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CCamera::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
	
	return S_OK;
}

void CCamera::Follow(const _float& fTimeDelta)
{
	if (m_pTargetObj == nullptr)
		return;

	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTargetObj->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, COMPONENTID::ID_STATIC));
	if (pTargetTransform == nullptr)
		return;

	_vec3 vTargetPos, vCameraPos;
	_vec3 vDir;

	m_pTransformCom->Get_Info(INFO_POS, &vCameraPos);
	pTargetTransform->Get_Info(INFO_POS, &vTargetPos);

	m_pTransformCom->Set_Info(INFO_LOOK, &vTargetPos);

	vDir = vTargetPos - vCameraPos;

	_float fLen = D3DXVec3Length(&vDir);
	if (fLen < m_fDist)
		return;
	
	D3DXVec3Normalize(&vDir, &vDir);
	vCameraPos.x = vTargetPos.x;
	vCameraPos.y = vTargetPos.y + 10.f;
	vCameraPos.z = vTargetPos.z - 20.f;
	m_pTransformCom->Set_Info(INFO_POS, &vCameraPos);
	
}



void CCamera::Key_Input(const _float & fTimeDelta)
{	
	POINT pt = { WINCX / 2, WINCY / 2 };
	ShowCursor(false);
	ClientToScreen(m_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CCamera::Mouse_Move(const _float & fTimeDelta)
{
	_vec3 vUp, vRight, vLook;
	_vec3  vDirRight, vDirLook, vDirUp;

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);

	D3DXVec3Normalize(&vDirRight, &vRight);
	D3DXVec3Normalize(&vDirLook, &vLook);
	D3DXVec3Normalize(&vDirUp, &vUp);


	if (_long dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->RotationAxis(vDirRight, D3DXToRadian(dwMouseMove * 5.f * fTimeDelta));
	}

	if (_long dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		m_pTransformCom->RotationAxis({ 0.f, 1.f, 0.f }, D3DXToRadian(dwMouseMove * 5.f * fTimeDelta));
	}

	if (_long dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Z))
	{
		m_fFov += dwMouseMove * fTimeDelta * 0.1f;

		if (m_fFov > D3DX_PI)
			m_fFov = D3DX_PI;

		if (m_fFov < 1.f)
			m_fFov = 1.f;
	}
}



CCamera* CCamera::Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar)
{
	CCamera*	pInstance = new CCamera(pGraphicDev);

	pInstance->Set_Handle(_hWnd);
	pInstance->Set_Near(fNear);
	pInstance->Set_Far(fFar);
	
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CCamera::Free()
{
	CGameObject::Free();
}
