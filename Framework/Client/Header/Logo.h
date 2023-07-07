#pragma once

#include "Scene.h"
#include "BackGround.h"

#include "Player.h"
#include "Monster.h"
#include "BlueBeatle.h"
#include "TrashBig.h"
#include "DesertRhino.h"
#include "SunGollem.h"
#include "Terrain.h"
#include	"PushStone.h"
#include "NearReactObj.h"
#include "GrabbableObj.h"
#include "NearReactMoonObj.h"
#include "BalpanObj.h"
#include "HitObj.h"
#include "BlockObj.h"
#include "TutorialNPC.h"
#include "NPCText.h"


class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_Environment(LAYER_TYPE _eType);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

