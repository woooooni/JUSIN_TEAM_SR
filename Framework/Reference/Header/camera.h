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
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void);

public:
	const _matrix	GetViewMatrix() { return m_pTransformCom->Get_WorldInverseMatrix(); }
	const _matrix& GetProjectionMatrix() { return m_matProj; }

	void		Set_Handle(HWND _hWnd) { m_hWnd = _hWnd; }
	void		Set_Near(_float _f) { m_fNear = _f; }
	void		Set_Far(_float _f) { m_fFar = _f; }
	void		Set_Fov(_float _f) { m_fFov = _f; }

	void		Set_TargetObj(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Follow(const _float& fTimeDelta);

public:
	void		CamShake(float _fDuration);
	void		FadeIn(float _fTime);
	void		FadeOut(float _fTime);
	void		Check_FadeOut();

	//private:
	//	void		CustomLookAtLH(_matrix* pOut, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	//	void		CustomPerspectiveLH(_matrix* pOut, const _float _fov, const _float _fAspect, const _float _fNear, const _float _fFar);

private:
	_matrix		m_matView;
	_matrix		m_matProj;

	_float		m_fNear;
	_float		m_fFar;
	_float		m_fFov;
	_float		m_fDist;
	_float		m_fFollowSpeed;

	CGameObject* m_pTargetObj;

	HWND		m_hWnd;

	list<tCamEffect>	m_lCamEffect;
	//////////////////////////////////////////
	_bool				m_bFadeOut = false;
	_float				m_fFadeInAmount = 0.f;
	//////////////////////////////////////////
	_float				m_fShakeForce;
	_float				m_fAlpha; // 0에서 1까지만 사용
	CTexture* m_pVeilTex;

public:
	static CCamera* Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar);
	virtual void		Free()	override;
};

END