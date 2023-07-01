#pragma once
#include "GameObject.h"
#include "Transform.h"

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
	const _matrix&	GetProjectionMatrix() { return m_matProj; }

	void		Set_Handle(HWND _hWnd) { m_hWnd = _hWnd; }
	void		Set_Near(_float _f) { m_fNear = _f; }
	void		Set_Far(_float _f)	{ m_fFar = _f; }
	void		Set_Fov(_float _f) { m_fFov = _f; }

	void		Set_TargetObj(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Follow(const _float& fTimeDelta);

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

public:
	static CCamera*		Create(HWND _hWnd, LPDIRECT3DDEVICE9 pGraphicDev, _float fNear, _float fFar);
	virtual void		Free()	override;
};

END