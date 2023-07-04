#pragma once
#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr(void);
	virtual ~CFontMgr(void);

public:
	HRESULT Ready_Font(HINSTANCE hInst, HWND hWnd);

private:
	vector<LPD3DXFONT> m_vecFont;


public:
	virtual void	Free(void);
};

END
#endif