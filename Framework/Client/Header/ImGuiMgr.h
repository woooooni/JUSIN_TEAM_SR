#pragma once

#include "Base.h"
#include "Engine_Define.h"


class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr);

public:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	HRESULT Ready_ImGuiMgr(HWND _hWnd, LPDIRECT3DDEVICE9 _pGraphicDev);
	void	Update_ImGui();
	void	Render_ImGui();
	
public:
	void OnOffImGui() { m_bEnabled = !m_bEnabled; }
	_bool IsEnabled() { return m_bEnabled; }


private:
	_bool m_bEnabled;
	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
	virtual void Free() override;
};

