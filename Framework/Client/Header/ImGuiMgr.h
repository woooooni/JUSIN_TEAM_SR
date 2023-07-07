#pragma once

#include "Base.h"
#include "Engine_Define.h"


class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

public:
	enum class TOOL_MODE{ OBJECT, TERRAIN, MAP, MODE_END };

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	HRESULT Ready_ImGuiMgr(HWND _hWnd, LPDIRECT3DDEVICE9 _pGraphicDev);
	void	Update_ImGui();
	void	Render_ImGui();
	
public:
	TOOL_MODE Get_CurrToolMode() { return m_eMode; }
	void Set_ToolMode(TOOL_MODE _eMode) { m_eMode = _eMode; }


public:
	void OnOffImGui() { m_bEnabled = !m_bEnabled; }
	_bool IsEnabled() { return m_bEnabled; }


private:
	void UpdateObjectTool();
	void UpdateTerrainTool();
	void UpdateMapTool();

private:
	_bool m_bEnabled;
	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	TOOL_MODE m_eMode;

public:
	virtual void Free() override;
};

