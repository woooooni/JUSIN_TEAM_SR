#pragma once
#include	"GameObject.h"
class CNueFlower : public CGameObject
{
	CLONE(CNueFlower);

protected:
	explicit CNueFlower(LPDIRECT3DDEVICE9 p_Dev);
	explicit CNueFlower(const CNueFlower& rhs);
	virtual ~CNueFlower();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CNueFlower* Create(LPDIRECT3DDEVICE9 p_Dev,  const _vec3& p_Pos = { 0, 0, 0 }, const OBJ_DIR pDir = OBJ_DIR::DIR_U);

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

protected:
	HRESULT		Ready_Component(void);

	_vec3 m_vmyPos;
	_vec3 m_voriginPos;
	_float	m_fMaxAngle;
	_float m_fCurAngle;

};

