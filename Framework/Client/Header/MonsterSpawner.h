#pragma once
#include "GameObject.h"
class CMonsterSpawner : public CGameObject
{
	CLONE(CMonsterSpawner)

protected:
	explicit CMonsterSpawner(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterSpawner(const CMonsterSpawner& rhs);
	virtual ~CMonsterSpawner();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

private:
	virtual HRESULT	Ready_Component(void);

public:
	void		Set_Spawn(_bool _bSpawn) { m_bSpawn = _bSpawn; m_fAccTime = (rand() % 10) * (m_fSpawnTime / 10.0f); }
	void		Set_Nexus(CGameObject* _pNexus) { m_pNexus = _pNexus; }
public:
	static	CMonsterSpawner* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;


private:
	void		SpawnMonsters();

	_bool m_bSpawn = false;

	_float m_fAccTime;
	_float m_fSpawnTime;

	CGameObject* m_pNexus = nullptr;

	_uint m_iMonsterCount;
	_uint m_iAccCount;

	_float m_fSpawnDelay;
};

