#include "Camera.h"
#include "Export_Function.h"
#include <cassert>
#include "TimerMgr.h"
#include "PickingMgr.h"
// Globals
const float fZoom = 5.0f;

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND _hWnd)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_CAMERA, OBJ_ID::CAMERA)
	, m_fFov(4.f)
	, m_pTargetObj(nullptr)
	, m_fDist(0.2f)
	, m_fFollowSpeed(5.f)
	, m_fShakeForce(2.f)
	, m_fNear(1.0f)
	, m_fFar(1000.0f)
	, m_fMoveSpeed(10.f)
	, m_eState(CAMERA_STATE::GAME)
	, m_hWnd(_hWnd)
	, m_vOffset({0.f, 5.f, -10.f})
{

}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_fFov(4.f)
	, m_fDist(rhs.m_fDist)
	, m_pTargetObj(rhs.m_pTargetObj)
	, m_fFollowSpeed(rhs.m_fFollowSpeed)
	, m_fShakeForce(rhs.m_fShakeForce)
	, m_fMoveSpeed(10.f)
	, m_eState(CAMERA_STATE::GAME)
	, m_hWnd(rhs.m_hWnd)
{

}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3(0.f, 5.f, -10.f));

	
	m_eState = CAMERA_STATE::GAME;
	m_fMoveSpeed = 10.0f;
	
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / m_fFov, WINCX / WINCY, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update_Object(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case CAMERA_STATE::GAME:
		Update_GameCamera(fTimeDelta);
		break;
	case CAMERA_STATE::TOOL:
		Update_ToolCamera(fTimeDelta);
		break;

	case CAMERA_STATE::CUT_SCENE:
		Update_CutSceneCamera(fTimeDelta);
		break;
	default:
		break;
	}


	if (m_pTargetObj)
		Check_Alpha();

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CCamera::LateUpdate_Object(void)
{
	switch (m_eState)
	{
	case CAMERA_STATE::GAME:
		LateUpdate_GameCamera();
		break;
	case CAMERA_STATE::TOOL:
		LateUpdate_ToolCamera();
		break;
	case CAMERA_STATE::CUT_SCENE:
		LateUpdate_CutSceneCamera();
		break;
	default:
		break;
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

	CTransform* pTargetTransform = m_pTargetObj->Get_TransformCom();
	if (pTargetTransform == nullptr)
		return;

	_vec3 vTargetPos, vCameraPos;
	_vec3 vDir;

	m_pTransformCom->Get_Info(INFO_POS, &vCameraPos);
	pTargetTransform->Get_Info(INFO_POS, &vTargetPos);

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	vDir = vTargetPos - vCameraPos;
	_float fLen = D3DXVec3Length(&vDir);

	m_fDist = 1.f;

	if (fLen < m_fDist)
		return;
	

	D3DXVec3Lerp(&vDir, &(vCameraPos), &(vTargetPos + m_vOffset), m_fFollowSpeed * fTimeDelta);
	vCameraPos = vDir;
	vLook = vCameraPos - m_vOffset;

	m_pTransformCom->Set_Info(INFO_POS, &vCameraPos);
	m_pTransformCom->Set_Info(INFO_LOOK, &vLook);


}

void CCamera::Check_Alpha()
{
	
	_vec3 vTargetPos;
	m_pTargetObj->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	D3DXVec3TransformCoord(&vTargetPos, &vTargetPos, &m_matView);

	_vec3 vRayPos = _vec3(0.f, 0.f, 0.f);
	_vec3 vRayDir = vTargetPos - vRayPos;

	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matCamWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matCamWorld);

	_matrix matObjWorld;
	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());

	

	const vector<CGameObject*>& vecObj = Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Get_GameObjectVec();
	for (size_t i = 0; i < vecObj.size(); ++i)
	{
		if (!vecObj[i]->Is_Active())
			continue;

		if (vecObj[i]->GetObj_Id() != OBJ_ID::PROP && vecObj[i]->GetObj_Id() != OBJ_ID::TREE)
			continue;

		vecObj[i]->Set_Alpha(255);
		
		CTransform* pObjTransform = vecObj[i]->Get_TransformCom();
		CVIBuffer* pObjBuffer = vecObj[i]->Get_BufferCom();

		if (pObjTransform == nullptr)
			continue;

		if (pObjBuffer == nullptr)
			continue;

		matObjWorld = *(pObjTransform->Get_WorldMatrix());

		_vec3 vTempRayPos, vTempDir;
		D3DXVec3TransformCoord(&vTempRayPos, &vRayPos, &pObjTransform->Get_WorldInverseMatrix());
		D3DXVec3TransformNormal(&vTempDir, &vRayDir, &pObjTransform->Get_WorldInverseMatrix());

		D3DXVec3Normalize(&vTempDir, &vTempDir);

		LPDIRECT3DVERTEXBUFFER9 objVB = pObjBuffer->GetBuffer();
		LPDIRECT3DINDEXBUFFER9	objIB = pObjBuffer->GetIndex();

		VTXTEX* pVB;
		INDEX32* pIB;
		objVB->Lock(0, 0, (void**)&pVB, 0);
		objIB->Lock(0, 0, (void**)&pIB, 0);

		_float fU, fV, fDist;
		_float fMinDistance = 999999.f;

		for (_uint cnt = 0; cnt < pObjBuffer->GetTriangleCount(); ++cnt)
		{
			if (TRUE == D3DXIntersectTri(
				&pVB[pIB[cnt]._0].vPosition,
				&pVB[pIB[cnt]._1].vPosition,
				&pVB[pIB[cnt]._2].vPosition, &vTempRayPos, &vTempDir, &fU, &fV, &fDist))
			{
				_vec3 vTargetPos, vAlphaPos;
				m_pTargetObj->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
				pObjTransform->Get_Info(INFO_POS, &vAlphaPos);
				if (vAlphaPos.z < vTargetPos.z)
				{
					vecObj[i]->Set_Alpha(60);
					break;
				}
			}
		}

		objVB->Unlock();
		objIB->Unlock();

	}

}

void CCamera::Update_GameCamera(const _float& fTimeDelta)
{
	//Key_Input_Game(fTimeDelta);
	Follow(fTimeDelta);
	Mouse_Move(fTimeDelta);

	_vec3 vPos, vLook, vRight, vUp;

	ZeroMemory(&vPos, sizeof(_vec3));
	ZeroMemory(&vLook, sizeof(_vec3));
	ZeroMemory(&vUp, sizeof(_vec3));

	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_POS, &vPos);

	D3DXMatrixLookAtLH(&m_matView, &vPos, &vLook, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / m_fFov, WINCX / WINCY, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCamera::LateUpdate_GameCamera()
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
			m_fShakeForce = effect.fShakeForce;
			_float fOffsetX = (std::rand() % 10) * 0.01f * m_fShakeForce;
			_float fOffsetY = (std::rand() % 10) * 0.01f * m_fShakeForce;

			// Look, Pos 둘 다 적용시켜야 움직임이 자연스러움
			vCamPos.x += fOffsetX;
			vCamPos.y += fOffsetY;

			vCamLook.x += fOffsetX;
			vCamLook.y += fOffsetY;

			D3DXMatrixLookAtLH(&m_matView, &vCamPos, &vCamLook, &vCamUp);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
		}

		//_uint iWidth = WINCX;
		//_uint iHeight = WINCY;

		//BLENDFUNCTION buffer = {};

		//buffer.BlendOp = AC_SRC_OVER;
		//buffer.BlendFlags = 0;
		//buffer.SourceConstantAlpha = (_byte)(255 * m_fAlpha);
		//buffer.AlphaFormat = 0;

		//if (CAM_EFFECT::FADE_IN == effect.eEffect || CAM_EFFECT::FADE_OUT == effect.eEffect)
		//{
		//	//m_pTransformCom->Set_Pos();
		//	// m_fNear의 위치에 Veil을 위치시킨다. 이후 알파블랜드
		//	if (CAM_EFFECT::FADE_IN == effect.eEffect)
		//	{
		//		m_fAlpha = effect.fDuration / effect.fCurTime;
		//	}

		//	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
		//	{
		//		m_fAlpha = 1.f - (effect.fDuration / effect.fCurTime);
		//	}

		//	//AlphaBlend(hdc, 0, 0, iWidth, iHeight, m_pVeilTex->DC GET하는 함수(),
		//	//	0, 0, iWidth, iHeight, buffer);
		//}

		effect.fCurTime += fTimeDelta;

		// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
		if (effect.fDuration <= effect.fCurTime)
			m_lCamEffect.pop_front(); // 효과를 종료한다
	}
}

void CCamera::Update_ToolCamera(const _float& fTimeDelta)
{

	Key_Input_Tool(fTimeDelta);

	_vec3 vPos, vLook, vRight, vUp;

	ZeroMemory(&vPos, sizeof(_vec3));
	ZeroMemory(&vLook, sizeof(_vec3));
	ZeroMemory(&vUp, sizeof(_vec3));

	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_LOOK, &vLook);

	vUp = { 0.0f, 1.0f, 0.0f };
	vLook = vPos + vLook;

	D3DXMatrixLookAtLH(&m_matView, &vPos, &vLook, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / m_fFov, WINCX / WINCY, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCamera::LateUpdate_ToolCamera()
{
	
}

void CCamera::Update_CutSceneCamera(const _float& fTimeDelta)
{
	Follow(fTimeDelta);

	_vec3 vPos, vLook, vRight, vUp;

	ZeroMemory(&vPos, sizeof(_vec3));
	ZeroMemory(&vLook, sizeof(_vec3));
	ZeroMemory(&vUp, sizeof(_vec3));

	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(MATRIX_INFO::INFO_POS, &vPos);

	D3DXMatrixLookAtLH(&m_matView, &vPos, &vLook, &vUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / m_fFov, WINCX / WINCY, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCamera::LateUpdate_CutSceneCamera()
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
			_float fOffsetX = (std::rand() % 10) * 0.01f * m_fShakeForce;
			_float fOffsetY = (std::rand() % 10) * 0.01f * m_fShakeForce;

			int iTemp = rand() % 2;
			if (iTemp == 0)
				fOffsetX *= -1.0f;

			iTemp = rand() % 2;
			if (iTemp == 0)
				fOffsetY *= -1.0f;

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
		buffer.SourceConstantAlpha = (_byte)(255 /* m_fAlpha*/);
		buffer.AlphaFormat = 0;

		if (CAM_EFFECT::FADE_IN == effect.eEffect || CAM_EFFECT::FADE_OUT == effect.eEffect)
		{
			//m_pTransformCom->Set_Pos();
			// m_fNear의 위치에 Veil을 위치시킨다. 이후 알파블랜드
			if (CAM_EFFECT::FADE_IN == effect.eEffect)
			{
				// m_fAlpha = effect.fDuration / effect.fCurTime;
			}

			if (CAM_EFFECT::FADE_OUT == effect.eEffect)
			{
				// m_fAlpha = 1.f - (effect.fDuration / effect.fCurTime);
			}

			//AlphaBlend(hdc, 0, 0, iWidth, iHeight, m_pVeilTex->DC GET하는 함수(),
			//	0, 0, iWidth, iHeight, buffer);
		}

		effect.fCurTime += fTimeDelta;

		// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
		if (effect.fDuration <= effect.fCurTime)
			m_lCamEffect.pop_front(); // 효과를 종료한다
	}
}

void CCamera::CamShake(float _fDuration, float _fShakeForce)
{
	if (0.f == _fDuration)
		assert(nullptr);

	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::SHAKE;
	effect.fDuration = _fDuration;
	effect.fCurTime = 0.f;
	effect.fShakeForce = _fShakeForce;

	m_lCamEffect.push_back(effect);
}

void CCamera::FadeIn(float _fTime)
{
	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::FADE_IN;
	effect.fCurTime = 0.f;
	effect.fDuration = _fTime;
	// m_fAlpha = 1.f;

	m_lCamEffect.push_back(effect);
}

void CCamera::FadeOut(float _fTime)
{
	tCamEffect effect = {};

	effect.eEffect = CAM_EFFECT::FADE_OUT;
	effect.fCurTime = 0.f;
	effect.fDuration = _fTime; // 지속시간
	// m_fAlpha = 0.f;

	m_lCamEffect.push_back(effect);
}

void CCamera::Key_Input_Tool(const _float& fTimeDelta)
{

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_long		dwMouseMove = 0;
		if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
		{
			_vec3	vRight;
			_vec3	vLook;

			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			_matrix		matRot;

			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_pTransformCom->Set_Info(INFO_LOOK, &vLook);
		}

		if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
		{
			_vec3	vUp{ 0.f, 1.f, 0.f };
			_vec3	vLook;
			_vec3	vRight;

			_matrix		matRot;

			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

			D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			D3DXVec3TransformNormal(&vRight, &vRight, &matRot);

			m_pTransformCom->Set_Info(INFO_LOOK, &vLook);
			m_pTransformCom->Set_Info(INFO_RIGHT, &vRight);
		}
	}

	


	if (KEY_HOLD(KEY::W))
	{
		_vec3 vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		m_pTransformCom->Move_Pos(&vDir, m_fMoveSpeed, fTimeDelta);
	}

	if (KEY_HOLD(KEY::A))
	{
		_vec3 vDir;
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		m_pTransformCom->Move_Pos(&vDir, -m_fMoveSpeed, fTimeDelta);
	}

	if (KEY_HOLD(KEY::D))
	{
		_vec3 vDir;
		m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
		m_pTransformCom->Move_Pos(&vDir, m_fMoveSpeed, fTimeDelta);
	}

	if (KEY_HOLD(KEY::S))
	{
		_vec3 vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		m_pTransformCom->Move_Pos(&vDir, -m_fMoveSpeed, fTimeDelta);
	}

	if (KEY_HOLD(KEY::Q) && KEY_NONE(KEY::CTRL))
	{
		m_pTransformCom->Move_Pos(&_vec3{0.f, 1.f, 0.f}, m_fMoveSpeed, fTimeDelta);
	}

	if (KEY_HOLD(KEY::E) && KEY_NONE(KEY::CTRL))
	{
		m_pTransformCom->Move_Pos(&_vec3{ 0.f, 1.f, 0.f }, -m_fMoveSpeed, fTimeDelta);
	}
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

	if (KEY_TAP(KEY::NUM_8))
		CamShake(1.f);

	if (KEY_TAP(KEY::F4))
	{
		m_vOffset *= 2.f;
	}

	if (KEY_TAP(KEY::F5))
	{
		m_vOffset /= 2.f;
	}
}

CCamera* CCamera::Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar)
{
	CCamera* pInstance = new CCamera(pGraphicDev, _hWnd);

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
