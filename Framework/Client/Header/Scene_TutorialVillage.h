#pragma once

#include "Scene.h"


class CScene_TutorialVillage : public Engine::CScene
{
private:
	explicit CScene_TutorialVillage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_TutorialVillage();

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


public:
	static CScene_TutorialVillage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

