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

class CPlayer_Skill_Range : public Engine::CGameObject
{
private:
	explicit CPlayer_Skill_Range(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Skill_Range(const CPlayer_Skill_Range& rhs);
	virtual ~CPlayer_Skill_Range();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	void				Reset_Range(void);
	void				Set_Range(_vec3& _vScale, _vec3& _vPos);
private:
	HRESULT			Ready_Component(void);
public:
	
	
public:
	// 충돌 호출
	virtual void Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID) override;
	virtual void Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID) override;



private:

public:
	static CPlayer_Skill_Range* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_matrix m_matReset;

private:
	virtual void Free() override;


};

