#include "..\..\Header\CameraMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
	m_vecCamera.reserve(10);
}

CCameraMgr::~CCameraMgr()
{
	Free();
}


CCamera * CCameraMgr::CreateCamera(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice, _float _fNear, _float _fFar)
{
	CCamera* pCamera = nullptr; 
	pCamera = CCamera::Create(_hWnd, _pDevice, _fNear, _fFar);
	
	NULL_CHECK_RETURN(pCamera, nullptr);
	m_vecCamera.push_back(pCamera);
	return pCamera;
}

CCamera * CCameraMgr::GetCamera(_uint _iIdx)
{
	if (m_vecCamera.size() >= _iIdx)
		return nullptr;

	return m_vecCamera[_iIdx];
}

void CCameraMgr::Free()
{
	
}
