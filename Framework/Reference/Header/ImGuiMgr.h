#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

public:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	HRESULT Ready_ImGui(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd);
	void	Update_ImGui();

public:
	void SetTarget(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }
	void OnOffImGui(bool _bEnabled)		{ m_bEnabled = _bEnabled; }
	bool IsEnabled()					{ return m_bEnabled; }

public:
	virtual void Free() override;

private:
	void DrawObjInfo();
	
private:
	bool			m_bEnabled;
	CGameObject*	m_pTargetObj;


};

END

