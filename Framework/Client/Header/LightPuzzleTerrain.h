#pragma once
#include "GameObject.h"
#include "Engine_Define.h"
#include	"LightPuzzle.h"

BEGIN(Engine)
class CRcTex;
END
struct LIGHT_INFO
{
	_uint		iIndexX;
	_uint		iIndexY;
	_bool		m_bIsOnTile = false;
	_bool		m_bIsLighting = false;
	CLightPuzzle* m_pOnTileObj = nullptr;

	list<_vec2> m_listLightDir;
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
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

public:
	void		Set_SubscribeEvent(_uint pEvent);
	const _vec3& Get_TilePos(_uint indexX, _uint indexY);

protected:
	HRESULT		Ready_Compnent();
	HRESULT		Make_LightRoute();

	_bool		m_bIsCanUpdate;
	_vec3* m_vTileCenterPos;
	vector<LIGHT_INFO*>		m_vecLightMap;
	CRcTex* m_pSubBuffer;
	_uint		tileX, tileY;
	

};

