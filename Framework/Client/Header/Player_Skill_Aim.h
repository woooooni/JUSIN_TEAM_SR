#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CPlayer_Skill_Aim : public CGameObject
{
	CLONE(CPlayer_Skill_Aim)
private:
	explicit CPlayer_Skill_Aim(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Skill_Aim(const CPlayer_Skill_Aim& rhs);
	virtual ~CPlayer_Skill_Aim();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);
public:


public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;



private:

public:
	static CPlayer_Skill_Aim* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

private:
	virtual void Free() override;


};

