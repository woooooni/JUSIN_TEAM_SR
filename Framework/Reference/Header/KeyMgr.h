#ifndef KeyMgr_h__
#define KeyMgr_h__

#include "GameObject.h"

BEGIN(Engine)
struct tKeyInfo
{
	KEY_STATE	eState;
	bool		bPrevPushed;
};

class  ENGINE_DLL CKeyMgr final : public CBase
{
	DECLARE_SINGLETON(CKeyMgr);

public:
	explicit CKeyMgr();
	virtual ~CKeyMgr() = default;

public:
	HRESULT Ready_KeyMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd);
	void	Update_KeyMgr();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	const POINT& GetMousePos() { return m_tMousePos; }
private:
	vector<tKeyInfo> m_vecKey;
	HWND	m_hWnd;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	POINT m_tMousePos;
public:
	virtual void Free() override;
};

END
#endif