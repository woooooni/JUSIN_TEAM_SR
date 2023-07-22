#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"

// class Texture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	SHAKE,
	NONE
};

struct tCamEffect
{
	CAM_EFFECT  eEffect;
	float		fDuration;
	float		fCurTime;
};

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
	CLONE(CCamera)
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND _hWnd);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void);

public:
	const _matrix GetViewMatrix() { return m_pTransformCom->Get_WorldInverseMatrix(); }
	const _matrix& GetProjectionMatrix() { return m_matProj; }

	void Set_Near(_float _f) { m_fNear = _f; }
	void Set_Far(_float _f) { m_fFar = _f; }

	void Set_Fov(_float _f) { m_fFov = _f; }
	_float Get_Fov() { return m_fFov; }

	void Set_TargetObj(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }

	void Set_Offset(const _vec3& _vOffset) { m_vOffset = _vOffset; }


private:
	HRESULT	Add_Component(void);

	void Key_Input_Tool(const _float& fTimeDelta);

	void Mouse_Move(const _float& fTimeDelta);

	void Update_GameCamera(const _float& fTimeDelta);
	void LateUpdate_GameCamera();

	void Update_ToolCamera(const _float& fTimeDelta);
	void LateUpdate_ToolCamera();

public:	 
	void Follow(const _float& fTimeDelta);
	void CamShake(float _fDuration);
	void FadeIn(float _fTime);
	void FadeOut(float _fTime);

	void		Set_CameraState(CAMERA_STATE _eState) { m_eState = _eState; if (_eState == CAMERA_STATE::TOOL) m_fMoveSpeed = 30.f; }
	CAMERA_STATE Get_CameraState() { return m_eState; }


private:
	_matrix		m_matView;
	_matrix		m_matProj;

	_float m_fNear;
	_float m_fFar;
	_float m_fFov;
	_float m_fDist;

	_float m_fMoveSpeed;
	_float m_fFollowSpeed;

	CGameObject* m_pTargetObj;

	HWND m_hWnd;

	list<tCamEffect> m_lCamEffect;
	_float m_fShakeForce;
	_float m_fAlpha;

	
	_vec3 m_vOffset;	// 타겟으로부터 떨어진 거리

	CAMERA_STATE m_eState;

public:
	static CCamera* Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar);
	virtual void	Free()	override;
};

END