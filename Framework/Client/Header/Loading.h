#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include	"Scene.h"
#include	"Stage1.h"
#include	"Logo.h"

class CLoading : public CBase
{
public:

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	SCENE_TYPE			Get_LoadingID() { return m_eID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() { return m_bFinish; }

public:
	static unsigned	int		CALLBACK Thread_Main(void* pArg);

public:
	HRESULT			Ready_Loading(SCENE_TYPE eLoadingID);
	_uint			Load_TutorialVillage();
	_uint			Load_Tool();


	CScene* Get_Scene() { return m_pLoadingScene; }



private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	SCENE_TYPE			m_eID;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	CScene*				m_pLoadingScene;

public:
	static		CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE eID);

private:
	virtual void		Free();

	HRESULT		Loading_Logo();
	HRESULT		Loading_Tool();


private:
	_uint	Load_Obj_Data(wstring _strFolderPath);
	_uint	Load_Terrain_Data(wstring _strFolderPath);


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

};

