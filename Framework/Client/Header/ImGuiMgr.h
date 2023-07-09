#pragma once

#include "Base.h"
#include "Engine_Define.h"


class CScene_Tool;

BEGIN(Engine)
class CGameObject;
END

class CImGuiMgr : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

public:
	enum class TOOL_MODE { OBJECT, TERRAIN, MAP, MODE_END };
	enum class OBJ_SELECTED { BLUE_BEATLE, DESERT_RHINO, TRASH_BIG, TILE };
private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr();

public:
	HRESULT Ready_ImGuiMgr(HWND _hWnd, LPDIRECT3DDEVICE9 _pGraphicDev);
	void	Update_ImGui(const _float& fTimeDelta);
	void	Render_ImGui();

public:
	TOOL_MODE Get_CurrToolMode() { return m_eMode; }
	void Set_ToolMode(TOOL_MODE _eMode) { m_eMode = _eMode; }

	void SetToolScene(CScene_Tool* pToolScene) { m_pToolScene = pToolScene; }

	void Set_Target(CGameObject* pGameObject) { m_pTargetObject = pGameObject; }

public:
	void OnOffImGui() { m_bEnabled = !m_bEnabled; }
	_bool IsEnabled() { return m_bEnabled; }


private:
	void Update_Help(const _float& fTimeDelta);
	void Update_Inspector(const _float& fTimeDelta);
	void Update_Hierachy(const _float& fTimeDelta);

	void UpdateObjectTool(const _float& fTimeDelta);
	void UpdateTerrainTool(const _float& fTimeDelta);
	void UpdateMapTool(const _float& fTimeDelta);
	void UpdateTileTool(const _float& fTimeDelta);

	void ResetSelectTarget();

	void CreateObj(OBJ_SELECTED _eSelected, _vec3& vHit);
	void DeleteObj();


	void Input(const _float& fTimeDelta);
	void ObjPicking();

private:
	_bool m_bEnabled;
	
	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	
	TOOL_MODE m_eMode;

	CScene_Tool* m_pToolScene;
	CGameObject* m_pTargetObject;
	CGameObject* m_pSelectedObject;
	
	wstring		m_strObjNaming;
	
	OBJ_SELECTED m_eSelectedObjType;
	_vec3 m_vObjScale;
	_uint m_iObjNum;

public:
	virtual void Free() override;
};

