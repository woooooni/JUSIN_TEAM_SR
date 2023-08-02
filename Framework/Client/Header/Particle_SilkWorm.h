#pragma once
#include "../Header/GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CParticle_SilkWorm : public CGameObject
{
    CLONE(CParticle_SilkWorm)

private:
	explicit CParticle_SilkWorm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle_SilkWorm(const CParticle_SilkWorm& rhs);
	virtual ~CParticle_SilkWorm();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void			Random_Particle(_vec3& _vPos);

private:
	HRESULT			Ready_Component(void);
	void			Update_Move(const _float& fTimeDelta);

public:
	static CParticle_SilkWorm* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_float m_fAccTime;
	_float m_fEndTime;
	_float m_fAccMoveTime;
	_float m_fMoveTime;

	_uint m_iAlpha;
	_uint m_iR;
	_uint m_iG;
	_uint m_iB;

private:
	virtual void Free() override;
};

