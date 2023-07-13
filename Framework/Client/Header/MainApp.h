#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CMainApp : public CBase 
{
private:		// 持失切, 社瑚切
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp();
	void		Render_MainApp();

private:
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT	Ready_Default_SamplerState();
	HRESULT	Ready_Default_RenderState();
	HRESULT	Ready_Proto_Component(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Proto_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementClass);
	HRESULT	Ready_Proto_Event();
	HRESULT Ready_Pool();
	

private:
	HRESULT Ready_Player_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Monster_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Boss_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_UI_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Item_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Effect_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_InteractionObj_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Environment_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Terrain_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_NPC_Texture(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CGraphicDev*			m_pGraphicDevClass;
	CManagement*			m_pManagementClass;

	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	static CMainApp*		Create();

private:
	virtual void		Free();

};

