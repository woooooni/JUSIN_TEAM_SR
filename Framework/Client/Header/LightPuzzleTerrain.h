#pragma once
#include "GameObject.h"

struct LIGHT_INFO
{
	_bool		m_bIsOnTile = false;
	_bool		m_bIsLighting = false;

	list<_vec3> m_listLightDir;
};

class CLightPuzzleTerrain : public CGameObject
{
	CLONE(CLightPuzzleTerrain)

protected:
	CLightPuzzleTerrain(LPDIRECT3DDEVICE9 p_Dev);
	CLightPuzzleTerrain(const CLightPuzzleTerrain& rhs);
	virtual ~CLightPuzzleTerrain();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CLightPuzzleTerrain* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& tileX, const _uint& tileY, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)override;
	virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	void		Set_SubscribeEvent(_uint pEvent);

protected:
	HRESULT		Ready_Compnent();
	_vec3* m_vTileCenterPos;
	vector<LIGHT_INFO*>		m_vecLightMap;

};

