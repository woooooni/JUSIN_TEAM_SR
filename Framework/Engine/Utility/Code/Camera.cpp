#include "Camera.h"
#include "Export_Function.h"
#include <cassert>
#include "TimerMgr.h"

// Globals
const float fZoom = 5.0f;

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_CAMERA)
	, m_fFov(4.f)
	, m_pTargetObj(nullptr)
	, m_fDist(10.f)
	, m_fFollowSpeed(5.f)
	, m_fShakeForce(2.f)
	, m_fAlpha(0.f)
	, m_pVeilTex(nullptr)
{

}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_fFov(4.f)
	, m_fDist(rhs.m_fDist)
	, m_pTargetObj(rhs.m_pTargetObj)
	, m_fFollowSpeed(rhs.m_fFollowSpeed)
	, m_fShakeForce(rhs.m_fShakeForce)
	, m_fAlpha(rhs.m_fAlpha)
	, m_pVeilTex(rhs.m_pVeilTex)
{

}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3(0.f, 5.f, -10.f));

	//if (nullptr == m_pVeilTex)
	//{
		// 전 픽셀이 검은색인 Texture 생성
		/*
		Near에 화면사이즈의 사각형을 만들어서 Alpha값을 조절한다.
		*/
		//}

	return S_OK;
}

_int CCamera::Update_Object(const _float& fTimeDelta)
{
	//Key_Input(fTimeDelta);
	Follow(fTimeDelta);
	Mouse_Move(fTimeDelta);

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

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return iExit;
}

void CCamera::LateUpdate_Object(void)
{

	float fTimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

	if (!m_lCamEffect.empty())
	{
		tCamEffect& effect = m_lCamEffect.front();

		// vLookAt
		_vec3 vCamPos, vCamLook, vCamUp;
		m_pTransformCom->Get_Info(MATRIX_INFO::INFO_POS, &vCamPos);
		m_pTransformCom->Get_Info(MATRIX_INFO::INFO_LOOK, &vCamLook);
		m_pTransformCom->Get_Info(MATRIX_INFO::INFO_UP, &vCamUp);

		if (CAM_EFFECT::SHAKE == effect.eEffect)
		{
			float fOffsetX = (std::rand() % 10) * 0.01f * m_fShakeForce;
			float fOffsetY = (std::rand() % 10) * 0.01f * m_fShakeForce;

			// Look, Pos 둘 다 적용시켜야 움직임이 자연스러움
			vCamPos.x += fOffsetX;
			vCamPos.y += fOffsetY;

			vCamLook.x += fOffsetX;
			vCamLook.y += fOffsetY;

			D3DXMatrixLookAtLH(&m_matView, &vCamPos, &vCamLook, &vCamUp);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		}

		_uint iWidth = WINCX;
		_uint iHeight = WINCY;

		BLENDFUNCTION buffer = {};

		buffer.BlendOp = AC_SRC_OVER;
		buffer.BlendFlags = 0;
		buffer.SourceConstantAlpha = (_byte)(255 * m_fAlpha);
		buffer.AlphaFormat = 0;

		if (CAM_EFFECT::FADE_IN == effect.eEffect || CAM_EFFECT::FADE_OUT == effect.eEffect)
		{
			//m_pTransformCom->Set_Pos();
			// m_fNear의 위치에 Veil을 위치시킨다. 이후 알파블랜드
			if (CAM_EFFECT::FADE_IN == effect.eEffect)
			{
				m_fAlpha = effect.fDuration / effect.fCurTime;
			}

			if (CAM_EFFECT::FADE_OUT == effect.eEffect)
			{
				m_fAlpha = 1.f - (effect.fDuration / effect.fCurTime);
			}

			//AlphaBlend(hdc, 0, 0, iWidth, iHeight, m_pVeilTex->DC GET하는 함수(),
			//	0, 0, iWidth, iHeight, buffer);
		}

		effect.fCurTime += fTimeDelta;

		// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
		if (effect.fDuration <= effect.fCurTime)
			m_lCamEffect.pop_front(); // 효과를 종료한다
	}

	__super::LateUpdate_Object();
}

void CCamera::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CCamera::Add_Component(void)
{
	/*
	정점을 이용한 텍스처출력시 Buffer와 TextureCom이 필요하다.
	*/

	CComponent* pComponent = nullptr;

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
	vCameraPos.y = vTargetPos.y + 5.f;
	vCameraPos.z = vTargetPos.z - 10.f;

	m_pTransformCom->Set_Info(INFO_POS, &vCameraPos);

}

void CCamera::CamShake(float _fDuration)
{
	if (0.f == _fDuration)
		assert(nullptr);

	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::SHAKE;
	effect.fDuration = _fDuration;
	effect.fCurTime = 0.f;

	m_lCamEffect.push_back(effect);
}

void CCamera::FadeIn(float _fTime)
{
	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::FADE_IN;
	effect.fCurTime = 0.f;
	effect.fDuration = _fTime;
	m_fAlpha = 1.f;

	m_lCamEffect.push_back(effect);
}

void CCamera::FadeOut(float _fTime)
{
	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::FADE_OUT;
	effect.fCurTime = 0.f;
	effect.fDuration = _fTime; // 지속시간
	m_fAlpha = 0.f;

	m_lCamEffect.push_back(effect);
}

void CCamera::Check_FadeOut()
{
	if (!m_bFadeOut)
		return;
}


void CCamera::Key_Input(const _float& fTimeDelta)
{
	POINT pt = { WINCX / 2, WINCY / 2 };
	ShowCursor(false);
	ClientToScreen(m_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CCamera::Mouse_Move(const _float& fTimeDelta)
{
	_vec3 vUp, vRight, vLook;
	_vec3  vDirRight, vDirLook, vDirUp;


	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);

	D3DXVec3Normalize(&vDirRight, &vRight);
	D3DXVec3Normalize(&vDirLook, &vLook);
	D3DXVec3Normalize(&vDirUp, &vUp);


	if (GetAsyncKeyState('9') & 0x8000)
	{
		if (m_fFov <= D3DX_PI / 8.0f)
			m_fFov = (D3DX_PI / 8.0f); // ( D3DX_PI / 8.0f)

		else
			m_fFov -= fZoom * fTimeDelta;
	}

	if (GetAsyncKeyState('0') & 0x8000)
	{
		if (m_fFov > D3DX_PI / 2.0f)
			m_fFov += fZoom * fTimeDelta;

		else
			m_fFov = (D3DX_PI / 2.0f);
	}

	if (GetAsyncKeyState('8') & 0x8000)
	{
		CamShake(10.f);
	}
}

CCamera* CCamera::Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar)
{
	CCamera* pInstance = new CCamera(pGraphicDev);

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
