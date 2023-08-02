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

	static			CNueFlower* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3& p_Pos = { 0, 0, 0 }, const _vec3& pDir = {1, 0, 0} , const _float& p_FirstLength = 1.f ,const _float & pLastLength = 1.f, LIGHT_TYPE _eType=LIGHT_TYPE::LIGHT_END);

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

protected:
	HRESULT		Ready_Component(void);

	_vec3 m_vmyPos;
	_vec3 m_voriginPos;
	_vec3 m_vDestination;
	_float	m_fMaxDistance;
	_float m_fFarSpeed;
	LIGHT_TYPE m_eType;
};

