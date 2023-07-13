#pragma once
#include "Player_State.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END


class CPlayer_Skill_Missile : public CPlayer_State
{
public:
	CPlayer_Skill_Missile(CGameObject* _pOwner);
	~CPlayer_Skill_Missile();


public:
	virtual HRESULT			Ready_State(void) override;
	virtual _int			Update_State(const _float& fTimeDelta) override;
	virtual void			LateUpdate_State(void) override;
	virtual void			Render_State(void) override;



private:
	HRESULT					Shoot(void);
	bool					Find_Target(void);
	void					Set_PlayerDir();

	_vec3					m_vDir;

	CGameObject* m_pTarget = nullptr;

	_float m_fShootTime;
	_float m_fAccShootTime;

	_uint m_iMaxBombCount;
	_uint m_iBombCount;

	_float m_fMaxLength;

	bool m_bSkillStart = false;
};

