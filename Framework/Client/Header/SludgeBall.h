#pragma once
#include "GameObject.h"
#include "MonsterAim.h"
#include "Bullet.h"

class CSludgeBall :
	public CBullet
{

	CLONE(CSludgeBall)

protected:
	explicit CSludgeBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSludgeBall(const CSludgeBall& rhs);
	virtual ~CSludgeBall();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT	Add_Component(void);



public:

	void Set_Dst(_vec3 _Dst) { m_vDst = _Dst; }
	static CSludgeBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	OBJ_DIR m_eDir;
	_vec3 m_vDst;

	CMonsterAim* m_pMonsterAim;

private:
	_float m_fMoveTime=20.f;
private:
	virtual void Free() override;

	void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);

};