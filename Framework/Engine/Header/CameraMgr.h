#pragma once

#include "Base.h"
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	CCamera* CreateCamera(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice, _float _fNear, _float _fFar);
	CCamera* GetMainCamera() { return m_vecCamera[0]; }
	CCamera* GetCamera(_uint _iIdx);
public:
	void Update_Camera();
	void LateUpdate_Camera();

private:
	vector<CCamera*> m_vecCamera;

public:
	virtual void Free() override;
};

END