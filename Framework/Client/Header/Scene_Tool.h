#pragma once

#include "Scene.h"

BEGIN(Engine)
class CCamera;
END
class CPlayer;
class CScene_Tool : public Engine::CScene
{
private:
	explicit CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Tool();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_Environment(LAYER_TYPE _eType);
	

public:
	void Save_Data();
	void Load_Data();
	CCamera* Get_MainCamera() { return m_pCamera; }
	CPlayer* Get_Player() { return m_pPlayer; }
private:
	Engine::CCamera* m_pCamera;
	CPlayer* m_pPlayer;
public:
	static CScene_Tool* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

