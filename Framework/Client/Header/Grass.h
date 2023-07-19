#pragma once
#include "FieldObject.h"

struct GrassTexture
{
	_vec2	m_vPosByCenter = { 0.f,0.f };
	_vec3	m_fScale = { 1, 1, 1 };
	_uint	m_iIsReverse = 0;
	_vec3	m_vRenderPos = { 0.f, 0.5f, 0.f };
	_float	m_fAngle = 0.f;
	_float	m_fOriginAngle = 0.f;
	_float	m_fMaxAngle = 0.f;
	CTexture* m_pTexture;
	_uint	m_iTextureIndex = 0;

};

class CGrass : public CFieldObject
{
public:
	CLONE(CGrass)

protected:
	CGrass(LPDIRECT3DDEVICE9 p_Dev);
	CGrass(const CGrass& rhs);
	virtual ~CGrass();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void		Add_Subscribe(_uint iNum);

	virtual void    Free() override;

	static			CGrass* Create(LPDIRECT3DDEVICE9 p_Dev, const GRASS_TYPE& p_Type, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

	virtual void Event_Start(_uint iEventNum) override;
	virtual void Event_End(_uint iEventNum)	override;

	list<GrassTexture*>& Get_TextureList() { return m_GrassList; }

	void		Add_DropItem(const ITEM_CODE& name, const _uint& percent)
	{
		m_dropItemMap.insert({ name, percent });
	}

	GRASS_TYPE Get_Type() { return m_eGrassType; }

	map<ITEM_CODE, _uint>& Get_ItemMap() { return m_dropItemMap; }

protected:

	HRESULT			Set_Grass();

	GRASS_TYPE		m_eGrassType;
	list<GrassTexture*>	m_GrassList;
	_float		m_fMaxMoveTime;
	_float		m_fCurMoveTime;
	_bool		m_bIsReverse;
	map<ITEM_CODE, _uint> m_dropItemMap;
};


