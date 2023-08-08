#pragma once

#include "Scene.h"
class CLoading;

class CScene_Loading final : public Engine::CScene
{
private:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextSceneType);
	virtual ~CScene_Loading();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	virtual HRESULT			Ready_Prototype();
	virtual HRESULT			Ready_Layer_Player()			override;
	virtual HRESULT			Ready_Layer_Camera()			override;
	virtual HRESULT			Ready_Layer_Terrrain()			override;
	virtual HRESULT			Ready_Layer_Environment()		override;
	virtual HRESULT			Ready_Layer_Monster()			override;
	virtual HRESULT			Ready_Layer_InterationObj()		override;
	virtual HRESULT			Ready_Layer_Effect()			override;
	virtual HRESULT			Ready_Layer_UI()				override;


private:
	SCENE_TYPE m_eNextScene;
	CLoading* m_pLoading;

	

public:
	static CScene_Loading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eNextScene);

private:
	virtual void Free() override;


};

