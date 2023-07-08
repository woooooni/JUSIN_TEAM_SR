#include "Export_System.h"
//////////////
inline LPD3DXFONT Get_Font()
{
	return CGraphicDev::GetInstance()->Get_Font();
}
//////////////
HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

inline LPDIRECT3DDEVICE9 Get_Device()
{
	return CGraphicDev::GetInstance()->Get_GraphicDev();
}

_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
 }
void			Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
 }
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

_bool	Ispermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->Ispermit_Call(pFrameTag, fTimeDelta);
}
HRESULT Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}


_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDevice::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDevice::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDevice::GetInstance()->Get_DIMouseMove(eMouseState);
}
HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDevice::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	CInputDevice::GetInstance()->Update_InputDev();
}


void			Release_System()
{
	CInputDevice::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}