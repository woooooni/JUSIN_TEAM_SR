#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CPlayer_Bullet_Lightning : public CGameObject
{
	CLONE(CPlayer_Bullet_Lightning)
private:
	explicit CPlayer_Bullet_Lightning(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Bullet_Lightning(const CPlayer_Bullet_Lightning& rhs);
	virtual ~CPlayer_Bullet_Lightning();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);
public:
	void			Shoot(_vec3& _vPos, _vec3& _vDir, _uint _iMax, _uint _iCurr);
	HRESULT			ShootNext();

public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;



private:

public:
	static CPlayer_Bullet_Lightning* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	_float m_fNextTime;
	_float m_fAccNextTime;

	_float m_fLightningTime;
	_float m_fAccLightningTime;

	_uint m_iMax;
	_uint m_iCurr;

	_vec3 m_vDir;

	bool m_bFinished = false;

private:
	virtual void Free() override;
};

