#pragma once

#include "Scene.h"

#include "Player.h"
#include "Monster.h"
#include "RedBeatle.h"
#include "BlueBeatle.h"
#include "GreenBeatle.h"
#include "TrashBig.h"
#include "TrashBummer.h"
#include "DesertRhino.h"
#include "SunGollem.h"
#include "Terrain.h"
#include "PushStone.h"
#include "NearReactObj.h"
#include "NearReactMoonObj.h"
#include "BalpanObj.h"
#include "HitObj.h"
#include "BlockObj.h"

#include "JellyStone.h"
#include "JellyCombined.h"
#include "Catapult.h"
#include "LightPuzzleTerrain.h"
#include "Coin.h"
#include "QuickSlot.h"
#include "NPCText.h"
#include "Icon.h"
#include "UI_Shop.h"
#include "UI_ItemInfo.h"
#include "UI_Cursor.h"
#include "UI_HPBar.h"
#include "UI_BossHP.h"
#include "UI_Totem.h"
#include "UI_ShortCutKey.h"
#include "UI_MonsterHP.h"
#include "LightPuzzleTerrain.h"
#include "LightPuzzleBase.h"
#include "LightPuzzlePiece.h"
#include	"LightFlower.h"
#include	"Turret.h"
#include	"ButterFly.h"
#include	"JellyBomb.h"
#include	"JellyBombCreator.h"
#include	"Grass.h"
#include	"NearReactMushroom.h"

#include "Player_Skill_Range.h"
#include "Player_Skill_Aim.h"

#include "Cupa.h"
#include "RollingBug.h"
#include "TrashSlime.h"
#include "TrashFast.h"
#include "Item_Hat_Monkey.h"

#include "SpitCactus.h"
#include "MothMage.h"
#include "SilkWorm.h"

#include "Item_Hat_Turtle.h"
#include "Item_Hat_Drill.h"
#include "Item_Hat_Light.h"
#include "Item_Hat_Mask.h"
#include "Item_Hat_Missile.h"
#include	"DefaultItem.h"
#include	"ClearBomb.h"
#include	"ClearField.h"
#include	"BreakStone.h"
#include	"HoleObj.h"
#include	"QuizStatue.h"


#include	"PlantCannon.h"

class CLogo final : public Engine::CScene
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
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

